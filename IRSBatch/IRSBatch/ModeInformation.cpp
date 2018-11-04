#include "stdafx.h"

#include "ModeInformation.h"

//////////////////////////////////////////////////////////////////////////


const int g_colors[] =
{
  0x8A2BE2FF,   //BlueViolet =                
  0xA52A2AFF,   //Brown =                     
  0x5F9EA0FF,   //CadetBlue =                 
  0x7FFF00FF,   //Chartreuse =                
  0xD2691EFF,   //Chocolate =                 
  0xFF7F50FF,   //Coral =                     
  0x6495EDFF,   //CornflowerBlue =                            
  0xDC143CFF,   //Crimson =                   
  0x00FFFFFF,   //Cyan =                      
  0x00008BFF,   //DarkBlue =                  
  0x008B8BFF,   //DarkCyan =                  
  0xB8860BFF,   //DarkGoldenrod =             
  0x006400FF,   //DarkGreen =                 
  0x8B008BFF,   //DarkMagenta =               
  0x556B2FFF,   //DarkOliveGreen =            
  0xFF8C00FF,   //DarkOrange =                
  0x9932CCFF,   //DarkOrchid =                
  0x8B0000FF,   //DarkRed =                   
  0x483D8BFF,   //DarkSlateBlue =             
  0x2F4F4FFF,   //DarkSlateGray =             
  0x00CED1FF,   //DarkTurquoise =             
  0x9400D3FF,   //DarkViolet =                
  0xFF1493FF,   //DeepPink =                  
  0x00BFFFFF,   //DeepSkyBlue =               
  0x696969FF,   //DimGray =                   
  0x1E90FFFF,   //DodgerBlue =                
  0xB22222FF,   //Firebrick =                             
  0x228B22FF,   //ForestGreen =               
  0xFF00FFFF,   //Fuchsia =                                          
  0x008000FF,   //Green =                                    
  0xFF69B4FF,   //HotPink =                   
  0xCD5C5CFF,   //IndianRed =                 
  0x4B0082FF,   //Indigo =                                                               
  0x20B2AAFF,   //LightSeaGreen =                                 
  0x32CD32FF,   //LimeGreen =                                     
  0xFF00FFFF,   //Magenta =                   
  0x800000FF,   //Maroon =                    
  0x0000CDFF,   //MediumBlue =                
  0xBA55D3FF,   //MediumOrchid =              
  0x3CB371FF,   //MediumSeaGreen =            
  0x7B68EEFF,   //MediumSlateBlue =           
  0x00FA9AFF,   //MediumSpringGreen =         
  0xC71585FF,   //MediumVioletRed =           
  0x191970FF,   //MidnightBlue =                                          
  0x000080FF,   //Navy =                      
  0x808000FF,   //Olive =                     
  0x6B8E23FF,   //OliveDrab =                 
  0xFFA500FF,   //Orange =                    
  0xFF4500FF,   //OrangeRed =                                  
  0xDB7093FF,   //PaleVioletRed =             
  0xCD853FFF,   //Peru =                      
  0x800080FF,   //Purple =                    
  0xFF0000FF,   //Red =                       
  0x4169E1FF,   //RoyalBlue =                 
  0x8B4513FF,   //SaddleBrown =               
  0xF4A460FF,   //SandyBrown =                
  0x2E8B57FF,   //SeaGreen =                  
  0xA0522DFF,   //Sienna =                    
  0x87CEEBFF,   //SkyBlue =                   
  0x708090FF,   //SlateGray =                 
  0x00FF7FFF,   //SpringGreen =               
  0x4682B4FF,   //SteelBlue =                 
  0x008080FF,   //Teal =                      
  0xFF6347FF,   //Tomato =                    
  0x40E0D0FF,   //Turquoise =                                  
  0x9ACD32FF,   //YellowGreen =            
};

const char g_names[] =
{
	  'A',
	  'B',
	  'C',
	  'D',
	  'E',
	  'F',
	  'G',
	  'H',
	  'I',
	  'J',
	  'K',
	  'L',
	  'M',
	  'N',
	  'O',
	  'P',
	  'Q',
	  'R',
	  'S',
	  'T',
	  'U',
	  'V',
	  'W',
	  'X',
	  'Y',
	  'Z'
	};

//////////////////////////////////////////////////////////////////////////

ModeInformation::ModeInformation()
	: m_next_color(0)
	, m_name_template(L"Cluster")
	, m_next_name_index(0)
	, m_next_point_index(0)
	, m_next_vset_index(0)
	, m_mouse_type(MOUSE_SIMPLE)
	, m_grid_divisions_number(10)
	, m_draw_class_decision_functions(false)
	, m_draw_class_division_functions(true)
	, m_log_on_each_iteration(true)
	, m_show_point_numbers(true)
{
	Flush();
}

ModeInformation::~ModeInformation()
{

}

void ModeInformation::Flush()
{
	m_next_core_point_index = m_next_point_index = m_next_name_index = m_next_color = 0;
	m_grid_x_scale[0] = -10;
	m_grid_x_scale[1] = 10;
	m_grid_divisions_number = 10;
	m_draw_class_division_functions = true;
	m_draw_class_decision_functions = false;
	m_log_on_each_iteration = true;
	m_show_point_numbers = true;
	m_point_radius = 7.0f;
}

void ModeInformation::SetCorePointNameTemplate(const std::wstring& i_template)
{
	m_name_template = i_template;
}

std::wstring ModeInformation::GetNextClusterName()
{
	std::wstring name = m_name_template;
	name += ' ';
	size_t names_count = sizeof(g_names) / sizeof(char);

	size_t digits = 1;

	if (true)
	{
		size_t temp = 1;
		while ((temp *= names_count) < m_next_point_index)
			++digits;
	}

	size_t index = m_next_name_index++;
	for (size_t i = 0; i < digits; ++i)
	{
		name += g_names[index%names_count];
		index /= names_count;
	}
	return name;
}

int ModeInformation::GetNextColorRGBA()
{
	return g_colors[m_next_color++ % (sizeof(g_colors) / sizeof(int))];
}

size_t ModeInformation::GetNextPointIndex()
{
	return ++m_next_point_index;
}

size_t ModeInformation::GetNextVSetIndex()
{
	return ++m_next_vset_index;
}

MouseType ModeInformation::GetMouseType()
{
	return m_mouse_type;
}

void ModeInformation::SetCurrentMouse(MouseType i_type)
{
	m_mouse_type = i_type;
}

double          ModeInformation::GetGridXScaleMin() const
{
	return m_grid_x_scale[0];
}

double          ModeInformation::GetGridXScaleMax() const
{
	return m_grid_x_scale[1];
}

bool            ModeInformation::DrawDecisionFunctions() const
{
	return m_draw_class_decision_functions;
}

bool            ModeInformation::DrawDivisionFunctions() const
{
	return m_draw_class_division_functions;
}

bool            ModeInformation::LogOnEachIteration() const
{
	return m_log_on_each_iteration;
}

size_t          ModeInformation::GetGridDivisionsNumber() const
{
	return m_grid_divisions_number;
}

void            ModeInformation::SetGridScale(double i_scale[2])
{
	m_grid_x_scale[0] = i_scale[0];
	m_grid_x_scale[1] = i_scale[1];
}

void            ModeInformation::SetDrawFunctions(bool i_draw_decision, bool i_draw_division)
{
	m_draw_class_decision_functions = i_draw_decision;
	m_draw_class_division_functions = i_draw_division;
}

void            ModeInformation::SetLog(bool i_log_on_each_iteration)
{
	m_log_on_each_iteration = i_log_on_each_iteration;
}

void            ModeInformation::SetGridDivisionsNumber(size_t i_number)
{
	m_grid_divisions_number = i_number;
}

void ModeInformation::SetShowPointNumbers(bool i_show_point_numbers)
{
	m_show_point_numbers = i_show_point_numbers;
}

bool ModeInformation::ShowPointNumbers() const
{
	return m_show_point_numbers;
}

void ModeInformation::SetPointRadius(float i_point_radius)
{
	m_point_radius = i_point_radius;
}

float ModeInformation::PointRadius() const
{
	return m_point_radius;
}