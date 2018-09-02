#include "stdafx.h"

#include "ImportExport.h"
#include "Database.h"

#include "ClassHandlerRenderer.h"
#include "IMode.h"
#include "MainFrm.h"
#include "ModeInformation.h"
#include "Vertex.h"
#include "DivisionFunctionsRenderer.h"

#include <algorithm>
#include <fstream>

namespace
{

} // namespace

namespace IRS
{

  bool ExportScene(const std::wstring& i_path)
  {
    std::wofstream file(i_path, std::ios::out);
    if (!file.good())
      return false;

    std::vector<ClassHandlerRenderer*> classes = IRS::DatabaseInstance->GetItems<ClassHandlerRenderer>();
    std::for_each(classes.begin(), classes.end(), [&file](ClassHandlerRenderer* ip_renderer)
    {
      file << *ip_renderer;
    });

    std::vector<Vertex*> vertices = IRS::DatabaseInstance->GetItems<Vertex>();
    std::for_each(vertices.begin(), vertices.end(), [&file](Vertex* ip_vertex)
    {
      file << *ip_vertex << std::endl;
    });

    file << std::endl << L"You can simply corrupt this file but aftermath will be on your conscience. " << std::endl
      << L"There was no aim to do this file encrypted and non-breakable." << std::endl
      << L"Regards" << std::endl;
    file.close();
    return true;
  }

  bool ImportScene(const std::wstring& i_path)
  {
    std::wifstream file(i_path, std::ios::in);
    if (!file.good())
      return false;
    file.seekg (0, file.beg);

    //set mode and flush it`s information
    CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();

    if (file.peek() == 'c')
      pMainWnd->GetController()->SetMode(MT_SIMPLE);
    else
      pMainWnd->GetController()->SetMode(MT_CLUSTERING);

    ModeInformation& mode_info = pMainWnd->GetController()->GetCurrentMode()->GetInformation();
    //clear information and data base
    mode_info.Flush();
    IRS::DatabaseInstance->Clear();

    MathBase::TClasses classes;
    MathBase::TPointCloud vertices;

    while (file)
    {
      unsigned short c = file.peek();

      if (c == 'c')
      {
        classes.push_back(new ClassHandler(L""));
        file >> *classes.back();
      }
      else if (c == 'v')
      {
        vertices.push_back(new Vector3D());
        file >> *vertices.back();
      }
      else
        {
        std::wstring temp;
        getline(file, temp);
        }
    }
    file.close();

    if (classes.size() != 0)
    {
      DivisionFunctionsRenderer* div_function_renderer = new DivisionFunctionsRenderer;
      IRS::DatabaseInstance->RegisterItem(div_function_renderer);
    }
    //now create renderer for all classes that were read
    while (!classes.empty())
    {
      ClassHandlerRenderer* clr = new ClassHandlerRenderer(mode_info.GetNextClusterName());
      ClassHandler* p_class_handler = classes.back();
      //add vertices
      const MathBase::TPointCloud& center_points = p_class_handler->GetCorePoints();
      for (size_t i = 0; i < center_points.size(); ++i)
        clr->AddCorePoint(new Vertex(*center_points[i], mode_info.GetNextPointIndex()));

      clr->SetColor(mode_info.GetNextColorRGBA());
      //register in database
      IRS::DatabaseInstance->RegisterItem(clr);
      delete classes.back();
      classes.pop_back();
    }

    //now create renderer for all vertices that were read
    while (!vertices.empty())
    {
      Vertex* vertex = new Vertex(*vertices.back(), mode_info.GetNextPointIndex());
      IRS::DatabaseInstance->RegisterItem(vertex);
      delete vertices.back();
      vertices.pop_back();
    }

    IRS::DatabaseInstance->Update();
    return true;
  }


} // IRS