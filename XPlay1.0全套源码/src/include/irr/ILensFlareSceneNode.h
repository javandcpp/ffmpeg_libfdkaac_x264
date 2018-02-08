#ifndef _ILENSFLARESCENENODE_H_INCLUDED_
#define _ILENSFLARESCENENODE_H_INCLUDED_
#include "ISceneNode.h"
namespace irr{
namespace scene{
class ILensFlareSceneNode : public ISceneNode {
public:
ILensFlareSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id, const core::vector3df& position):ISceneNode(parent, mgr){}
virtual video::SMaterial& getMaterial(u32 i) = 0;
virtual u32 getMaterialCount()const = 0;
virtual void OnRegisterSceneNode() = 0;
virtual void render() = 0;
};}}
#endif
