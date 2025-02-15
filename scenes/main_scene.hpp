#pragma once
#include <core/scene/scene.hpp>

namespace engine3d{
    class MainScene : public SceneScope{
    public:
        MainScene();
        MainScene(const std::string& Tag);

        virtual ~MainScene(){}


    private:
        void OnUpdate();
        void OnUIUpdate();

        void OnSceneRender();

    private:
        // bool m_IsMeshFilepathSelected=false;
        std::string m_MeshFilepath="";
        Ref<SceneObject> m_MainCamera=nullptr;
        Ref<SceneObject> m_Sphere=nullptr;
    };
};