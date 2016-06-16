/*
 *  $Id$
 */
#include "TU/v/vIIDC++.h"
#include "TU/v/ModalDialog.h"

namespace TU
{
namespace v
{
/************************************************************************
*  class MyModalDialog							*
************************************************************************/
class MyModalDialog : public ModalDialog
{
  public:
    typedef IIDCCamera::Format_7_Info	Format_7_Info;
    typedef IIDCCamera::PixelFormat	PixelFormat;
    
  private:
    enum	{c_U0, c_V0, c_Width, c_Height, c_PixelFormat, c_OK};

  public:
    MyModalDialog(Window& parentWindow, const Format_7_Info& fmt7info)	;
    
    PixelFormat		getROI(u_int& u0, u_int& v0,
			       u_int& width, u_int& height)		;
    virtual void	callback(CmdId id, CmdVal val)			;

  private:
    static CmdDef*	createROICmds(const Format_7_Info& fmt7info)	;
    
  private:
    const Format_7_Info&	_fmt7info;
};
    
MyModalDialog::MyModalDialog(Window& parentWindow,
			     const Format_7_Info& fmt7info)
    :ModalDialog(parentWindow, "ROI for Format_7_x", createROICmds(fmt7info)),
     _fmt7info(fmt7info)
{
}
    
IIDCCamera::PixelFormat
MyModalDialog::getROI(u_int& u0, u_int& v0, u_int& width, u_int& height)
{
    show();
    u0		= pane().getValue(c_U0);
    v0		= pane().getValue(c_V0);
    width	= pane().getValue(c_Width);
    height	= pane().getValue(c_Height);

    return IIDCCamera::uintToPixelFormat(pane().getValue(c_PixelFormat));
}

void
MyModalDialog::callback(CmdId id, CmdVal val)
{
    switch (id)
    {
      case c_U0:
      {
	int u0 = _fmt7info.unitU0
	       * ((int(val) + _fmt7info.unitU0/2) / _fmt7info.unitU0);
	pane().setValue(c_U0, u0);
      }
	break;
      case c_V0:
      {
        int v0 = _fmt7info.unitV0
	       * ((int(val) + _fmt7info.unitV0/2) / _fmt7info.unitV0);
	pane().setValue(c_V0, v0);
      }
	break;
      case c_Width:
      {
        int w = _fmt7info.unitWidth
	      * ((int(val) + _fmt7info.unitWidth/2) / _fmt7info.unitWidth);
	pane().setValue(c_Width, w);
      }
	break;
      case c_Height:
      {
        int h = _fmt7info.unitHeight
	      * ((int(val) + _fmt7info.unitHeight/2) / _fmt7info.unitHeight);
	pane().setValue(c_Height, h);
      }
	break;
	
      case c_OK:
	hide();
	break;
    }
}

CmdDef*
MyModalDialog::createROICmds(const Format_7_Info& fmt7info)
{
    static int			prop[4][3];
    static constexpr struct
    {
	IIDCCamera::PixelFormat	pixelFormat;
	const char*		name;
    } pixelFormats[] =
    {
	{IIDCCamera::MONO_8,		"Y(mono)"},
	{IIDCCamera::YUV_411,		"YUV(4:1:1)"},
	{IIDCCamera::YUV_422,		"YUV(4:2:2)"},
	{IIDCCamera::YUV_444,		"YUV(4:4:4)"},
	{IIDCCamera::RGB_24,		"RGB"},
	{IIDCCamera::MONO_16,		"Y(mono16)"},
	{IIDCCamera::RGB_48,		"RGB(color48)"},
	{IIDCCamera::SIGNED_MONO_16,	"Y(signed mono16)"},
	{IIDCCamera::SIGNED_RGB_48,	"RGB(signed color48)"},
	{IIDCCamera::RAW_8,		"RAW(raw8)"},
	{IIDCCamera::RAW_16,		"RAW(raw16)"}
    };
    static constexpr size_t	NPIXELFORMATS = sizeof(pixelFormats)
					      / sizeof(pixelFormats[0]);
    static MenuDef		pixelFormatMenus[NPIXELFORMATS + 1];
    static CmdDef		cmds[] =
    {
	{C_Slider, c_U0,     static_cast<int>(fmt7info.u0),
	 "    u0", prop[0], CA_None, 0, 0, 1, 1, 0},
	{C_Slider, c_V0,     static_cast<int>(fmt7info.v0),
	 "    v0", prop[1], CA_None, 0, 1, 1, 1, 0},
	{C_Slider, c_Width,  static_cast<int>(fmt7info.width),
	 " width", prop[2], CA_None, 0, 2, 1, 1, 0},
	{C_Slider, c_Height, static_cast<int>(fmt7info.height),
	 "height", prop[3], CA_None, 0, 3, 1, 1, 0},
	{C_ChoiceMenuButton, c_PixelFormat,
	 0, "pixel format", pixelFormatMenus, CA_None, 0, 4, 1, 1, 0},
	{C_Button, c_OK,     0,
	 "OK",	noProp,	 CA_None, 0, 5, 1, 1, 0},
	EndOfCmds
    };

  // Create commands for setting ROI.
    cmds[0].val = fmt7info.u0;
    prop[0][0]  = 0;
    prop[0][1]  = fmt7info.maxWidth - 1;
    prop[0][2]  = 1;
    cmds[1].val = fmt7info.v0;
    prop[1][0]  = 0;
    prop[1][1]  = fmt7info.maxHeight - 1;
    prop[1][2]  = 1;
    cmds[2].val = fmt7info.width;
    prop[2][0]  = 0;
    prop[2][1]  = fmt7info.maxWidth;
    prop[2][2]  = 1;
    cmds[3].val = fmt7info.height;
    prop[3][0]  = 0;
    prop[3][1]  = fmt7info.maxHeight;
    prop[3][2]  = 1;
    
  // Create a menu button for setting pixel format.
    size_t	npixelformats = 0;
    for (const auto& pixelFormat : pixelFormats)
	if (fmt7info.availablePixelFormats & pixelFormat.pixelFormat)
	{
	    pixelFormatMenus[npixelformats].label = pixelFormat.name;
	    pixelFormatMenus[npixelformats].id	  = pixelFormat.pixelFormat;
	    pixelFormatMenus[npixelformats].checked
		= (fmt7info.pixelFormat == pixelFormat.pixelFormat);
	    pixelFormatMenus[npixelformats].submenu = noSub;
	    ++npixelformats;
	}
    pixelFormatMenus[npixelformats].label = 0;
    
    return cmds;
}

/************************************************************************
*  global functions							*
************************************************************************/
bool
setSpecialFormat(IIDCCamera& camera, u_int id, int val, Window& window)
{
    switch (id)
    {
      case IIDCCamera::Format_7_0:
      case IIDCCamera::Format_7_1:
      case IIDCCamera::Format_7_2:
      case IIDCCamera::Format_7_3:
      case IIDCCamera::Format_7_4:
      case IIDCCamera::Format_7_5:
      case IIDCCamera::Format_7_6:
      case IIDCCamera::Format_7_7:
      {
	auto	format7 = IIDCCamera::uintToFormat(id);
	v::MyModalDialog
		modalDialog(window, camera.getFormat_7_Info(format7));
	u_int	u0, v0, width, height;
	auto	pixelFormat = modalDialog.getROI(u0, v0, width, height);
	camera.setFormat_7_ROI(format7, u0, v0, width, height)
	      .setFormat_7_PixelFormat(format7, pixelFormat)
	      .setFormatAndFrameRate(format7, IIDCCamera::uintToFrameRate(val));
      }
	return true;
    }
    
    return false;
}

bool
setSpecialFormat(const Array<IIDCCamera*>& cameras,
		 u_int id, int val, Window& window)
{
    switch (id)
    {
      case IIDCCamera::Format_7_0:
      case IIDCCamera::Format_7_1:
      case IIDCCamera::Format_7_2:
      case IIDCCamera::Format_7_3:
      case IIDCCamera::Format_7_4:
      case IIDCCamera::Format_7_5:
      case IIDCCamera::Format_7_6:
      case IIDCCamera::Format_7_7:
      {
	auto	format7 = IIDCCamera::uintToFormat(id);
	v::MyModalDialog
		modalDialog(window, cameras[0]->getFormat_7_Info(format7));
	u_int	u0, v0, width, height;
	auto	pixelFormat = modalDialog.getROI(u0, v0, width, height);
	for (auto camera : cameras)
	    camera->setFormat_7_ROI(format7, u0, v0, width, height)
		   .setFormat_7_PixelFormat(format7, pixelFormat)
		   .setFormatAndFrameRate(format7,
					  IIDCCamera::uintToFrameRate(val));
      }
	return true;
    }
    
    return false;
}

}
}
