#pragma once

#include "AssetLoader.h"
#include <sofa/core/loader/MeshLoader.h>

namespace sofa
{
namespace qtquick
{

template<class T>
struct MeshLoaderCreator : public TBaseSofaLoaderCreator<T>
{
    core::objectmodel::BaseObject::SPtr createInstance() {
        return core::objectmodel::New<T>();
    }
};

class MeshLoader : public AssetLoader
{
public:
    MeshLoader(std::string path, std::string extension);

    static const QUrl iconPath;
};

} // namespace qtquick
} // namespace sofa