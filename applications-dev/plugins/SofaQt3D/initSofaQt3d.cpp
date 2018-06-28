#include <sofa/core/ObjectFactory.h>

#include "SofaQt3D.h"

namespace sofa
{

namespace component
{

//Here are just several convenient functions to help user to know what contains the plugin

extern "C" {
    SOFA_SOFAQT3D_API void initExternalModule();
    SOFA_SOFAQT3D_API const char* getModuleName();
    SOFA_SOFAQT3D_API const char* getModuleVersion();
    SOFA_SOFAQT3D_API const char* getModuleLicense();
    SOFA_SOFAQT3D_API const char* getModuleDescription();
    SOFA_SOFAQT3D_API const char* getModuleComponentList();
}

void initExternalModule()
{
    static bool first = true;
    if (first)
    {
        first = false;
        //sofa::core::ObjectFactory::getInstance()->dump();
    }
}

const char* getModuleName()
{
    return "SofaQt3D";
}

const char* getModuleVersion()
{
    return "0.1";
}

const char* getModuleLicense()
{
    return "LGPL";
}


const char* getModuleDescription()
{
    return "A Sofa plugin for Qt3D";
}

const char* getModuleComponentList()
{
    /// string containing the names of the classes provided by the plugin
    static std::string classes = sofa::core::ObjectFactory::getInstance()->listClassesFromTarget(sofa_tostring(SOFA_TARGET));
    return classes.c_str();
}

} // namespace component

} // namespace sofa


