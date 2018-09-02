#pragma once

#include <functional>

struct Color
  {
  unsigned int m_color;//in RGBA

  Color()
    : m_color(0)
    {    }

  Color::Color(int i_color)
    : m_color(i_color)
    {  }

  Color(unsigned char i_r, unsigned char i_g, unsigned char i_b, unsigned char i_a)
    : m_color( (i_r << 24) | (i_g<<16) | (i_b << 8) | i_a )
    {    }

  template <typename TColorFrom>
  void Convert(TColorFrom i_color_from);

  template <typename TColorTo>
  TColorTo Convert();

  unsigned int operator >> (size_t i_shift)
    {
    return m_color >> i_shift;
    }
  };


class IRenderer
  {
  protected:
    std::function<void()> m_draw_function;
    Color m_clear_color;

  public:
    virtual ~IRenderer(){}

    virtual void Initialize() = 0;
    virtual void Release() = 0;
    virtual void Reshape() = 0;

    virtual void RenderLine(const Vector3D& i_first_point, const Vector3D& i_second_point, Color i_color, float i_width = 1.0) = 0;
    virtual void RenderRectangle(const Vector3D& i_center, double i_width, double i_height, Color i_color) = 0;
    virtual void RenderCircle(const Vector3D& i_center, double i_radius, Color i_color) = 0;
    virtual void RenderText(const Vector3D& i_position, const std::wstring& i_text, Color i_color) = 0;
    virtual void RenderText(const Vector3D& i_position, const std::string& i_text, Color i_color) = 0;

    virtual void RenderScene() = 0;
    virtual CRect GetTargetRectangle() = 0;

    void         SetClearColor(Color i_color)
      {
      m_clear_color = i_color;
      }
    void          SetDrawSceneFunction(std::function<void()> i_draw_function)
      {
      m_draw_function = i_draw_function;
      }

  };

namespace IRS
  {
  const Color CL_GRAY = Color(240,240,240,150);
  const Color CL_BLACK = Color(0,0,0,255);
  }