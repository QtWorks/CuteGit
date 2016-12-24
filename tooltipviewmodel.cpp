#include "tooltipviewmodel.h"

TooltipViewModel::TooltipViewModel() : QObject()
  ,m_x(0)
  ,m_y(0)
  ,m_visible(false)
  ,m_viewport(nullptr)
{

}
