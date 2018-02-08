#ifndef __I_DECAL_SCENE_NODE_H_INCLUDED__
#define __I_DECAL_SCENE_NODE_H_INCLUDED__
#include "ISceneNode.h"
namespace irr{
namespace scene{
class IDecalSceneNode : public ISceneNode{
public:
IDecalSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, core::triangle3df tri,
core::vector3df intersection, video::ITexture* image, float size):ISceneNode(parent, mgr) {}
virtual void setLifeSpan(double seconds) = 0;
virtual const core::dimension2d<f32>& getSize() = 0;
};}}
#endif
