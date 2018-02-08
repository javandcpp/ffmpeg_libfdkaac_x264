#ifndef IGUICOMPASS_H
#define IGUICOMPASS_H
#include "irrlicht.h"
#include "IGUIElement.h"
#include "ISceneNode.h"
namespace irr{
namespace gui{
class IGUICompass : public IGUIElement{
  public:
    IGUICompass(core::rect<s32> rect, IGUIEnvironment* env, IGUIElement* parent, bool rotateNeedle = true, video::E_MATERIAL_TYPE materialtype = video::EMT_TRANSPARENT_ALPHA_CHANNEL) : IGUIElement(EGUIET_ELEMENT, env, parent, -1, rect){}
    //! set body of compass texture
    virtual void setBodyTexture(video::ITexture* texture) = 0;
    //! set needle of compass texture
    virtual void setNeedleTexture(video::ITexture* texture) = 0;
    //! set compass heading
    virtual void setHeading(f32 deg) = 0;
    //! set object to get heading from
    virtual void setHeadingNode(scene::ISceneNode* node) = 0;
    //! set if the body or the needle shall rotate
    virtual void setRotateNeedle(bool rotateNeedle) = 0;
    //! get if the body or the needle shall rotate
    virtual bool getRotateNeedle() = 0;
    //! get the material of the body
    virtual video::SMaterial* getBodyMaterial() = 0;
    //! get the material of the needle
    virtual video::SMaterial* getNeedleMaterial() = 0;
    //! sets the size
    virtual void setSize(core::dimension2d<s32> size) = 0;
    //! sets the position
    virtual void setPosition(core::position2d<s32> pos) = 0;
    //! sets the rectangle
    virtual void setRect(core::rect<s32> rect) = 0;
};
}} // end namespaces
#endif // IGUICOMPASS_H
