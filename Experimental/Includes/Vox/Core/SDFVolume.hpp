/*************************************************************************
> File Name: SDFVolume.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References: http://graphicsrunner.blogspot.com/2009/01/volume-rendering-101.html
> Purpose: Signed distance field class for volume rendering with ray-casting technique.
> Created Time: 2020/08/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_SDF_VOLUME_HPP
#define CUBBYFLOW_VOX_SDF_VOLUME_HPP

#include <Vox/Utils/GLTypes.hpp>
#include <Core/Grid/ScalarGrid3.hpp>
#include <memory>

namespace Vox {

    class FrameContext;
    class Mesh;
    class RenderableObject;

    /**
     * Signed Distance Field class for volume rendering with ray-casting technique.
     * 
     * Construct volume with given Signed Distance Field which is generated through 
     * CubbyFlow solver or deserialized stuffs of the offline cache file.
     * 
     * The generated volume will be ray-casted through bounding box.
    */
    class SDFVolume
    {
    public:
        //! Default Constructor.
        SDFVolume();
        //! Default destructor.
        ~SDFVolume();
        //! Build volume 
        void BuildVolume(const std::shared_ptr<FrameContext>& ctx, CubbyFlow::ScalarGrid3Ptr sdf);

    protected:
        
    private:
        CubbyFlow::ScalarGrid3Ptr _sdf;
        std::unique_ptr<RenderableObject> _boundingBox;
    };

};

#endif