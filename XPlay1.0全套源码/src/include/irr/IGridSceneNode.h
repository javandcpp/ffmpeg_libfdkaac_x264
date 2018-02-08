#ifndef __I_GRID_SCENE_NODE_H__
#define __I_GRID_SCENE_NODE_H__
#include "irrlicht.h"
namespace irr{
namespace scene{
//! Grid scene node
/*! If you need a grid on the XY or ZY axis, simply rotate this node by 90
degrees in the appropiate axis.
This node creates an XZ grid by default, which should be fine for normal use.
Axis Lines are a default Red and Blue for the X and Z axis respectively.
Thanks goes to MasterGod for helping to clean up the code and for a few
bug fixes. */
class IGridSceneNode : public ISceneNode{
public:
	//! Constructor
	IGridSceneNode(ISceneNode* parent, ISceneManager* smgr, s32 id = -1,
		u32 spacing = 10, u32 size = 1000, video::SColor gridcolor = video::SColor(255,128,128,128), u32 accentlineoffset = 10,
		video::SColor accentgridcolor = video::SColor(255,192,192,192), bool axislinestate = false):ISceneNode(parent,smgr,id){
			if(Parent) Parent->addChild(this);
			updateAbsolutePosition();
  }
	//! Will create a copy of this scenenode and it's settings
	virtual IGridSceneNode* Clone(ISceneNode* newParent = 0, ISceneManager* newSceneManager = 0)=0;
	//! Pre-Register stuff
	virtual void OnRegisterSceneNode()=0;
	//! Render our grid using 3D lines
	virtual void render()=0;
	//! Returns our bounding box
	virtual const core::aabbox3d<f32>& getBoundingBox() const=0;
	//! Returns the total number of materials, in this case, only 1
	virtual u32 getMaterialCount()=0;
	//! Returns the only material
	virtual video::SMaterial& getMaterial(u32 i)=0;
	//! Returns the Spacing of the grid
	virtual u32 GetSpacing()=0;
	//! Returns the total size of the grid
	virtual u32 GetSize()=0;
	//! Returns the Grid Color
	virtual video::SColor GetGridColor()=0;
	//! Returns the offset of the accent lines
	virtual u32 GetAccentlineOffset()=0;
	//! Returns the Accent Line Color
	virtual video::SColor GetAccentlineColor()=0;
	//! Returns the Active State of the Axis Lines
	virtual bool AreAxisLineActive()=0;
	//! Returns the Color of the "X" axis lines
	virtual video::SColor GetAxisLineXColor()=0;
	//! Returns the Color of the "Y" axis lines
	virtual video::SColor GetAxisLineYColor()=0;
	//! Returns the Color of the "Z" axis lines
	virtual video::SColor GetAxisLineZColor()=0;
	//! Sets Spacing
	virtual void SetSpacing(u32 newspacing)=0;
	//! Sets Size
	virtual void SetSize(u32 newsize)=0;
	//! Sets the general grid color
	virtual void SetGridColor(video::SColor newcolor)=0;
	//! Sets the offset for the accent lines
	virtual void SetAccentlineOffset(u32 newoffset)=0;
	//! Sets the color of the accent lines
	virtual void SetAccentlineColor(video::SColor newcolor)=0;
	//! Sets the Active State of the Axis Lines
	virtual void SetAxisLineActive(bool active)=0;
	//! Sets the Color of the "X" axis lines
	virtual void SetAxisLineXColor(video::SColor XLine)=0;
	//! Sets the Color of the "Y" axis lines
	virtual void SetAxisLineYColor(video::SColor YLine)=0;
	//! Sets the Color of the "Z" axis lines
	virtual void SetAxisLineZColor(video::SColor ZLine)=0;
	//! Sets the Length of the axis lines
	virtual void SetAxisLineLength(f32 length)=0;
	//! Allows for setting a complete new material
	virtual void SetMaterial(video::SMaterial newMaterial)=0;
};
} // end namespace scene
} // end namespace irr
#endif // __I_GRID_SCENE_NODE_H__
