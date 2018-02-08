#ifndef IGUIProgressBar_H
#define IGUIProgressBar_H
#include "IGUIElement.h"
#include "irrTypes.h"
#include "SColor.h"
#include "IGUIFont.h"
namespace irr{
namespace gui{
class IGUIProgressBar : public IGUIElement{
public:
IGUIProgressBar(bool horizontal, bool border, bool background, IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle)
: IGUIElement(EGUIET_ELEMENT, environment, parent, id, rectangle){}
virtual void setMax(s32 m) = 0;
virtual void setPos(s32 p) = 0;
virtual void setColorBack(video::SColor color) = 0;
virtual void setColorFront(video::SColor color) = 0;
virtual void setColorText(video::SColor color) = 0;
virtual void setFontText(IGUIFont* font) = 0;
};
}}
#endif
