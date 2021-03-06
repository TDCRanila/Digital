#include <Editor/MainDockStage.h>

#include <imgui/imgui_internal.h>

// https://gist.github.com/PossiblyAShrub/0aea9511b84c34e191eaa90dd7225969

namespace DEditor
{

    DockerStage::DockerStage(std::string a_stage_name, bool a_start_disabled)
        : DCore::StageBase(a_stage_name, a_start_disabled)
        , _first_time_start(true)
        , _show_imgui_showcase(false)
    {

    }

    void DockerStage::RenderImGui()
    {
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= /*ImGuiWindowFlags_NoTitleBar*/ /*| ImGuiWindowFlags_NoCollapse | */ ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", nullptr, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        // Menubar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Explore"))
            {
                ImGui::MenuItem("Exit", NULL);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Editor Options"))
            {
                ImGui::MenuItem("Show ImGui Showcase", NULL, &_show_imgui_showcase);

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            if (_first_time_start)
            {
                _first_time_start = false;

                ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
                ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
                //  window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
                // out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
                // splitting these nodes should be/are done in correct order
                auto dock_id_top    = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.05f, nullptr, &dockspace_id);
                auto dock_id_right  = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.15f, nullptr, &dockspace_id);
                auto dock_id_bottom = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);
                auto dock_id_left   = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.15f, nullptr, &dockspace_id);

                auto dock_id_up     = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.05f, nullptr, &dockspace_id);
                auto dock_id_down   = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);

                auto dock_id_center = ImGui::DockBuilderGetCentralNode(dockspace_id)->ID;

                // we now dock our windows into the docking node we made above
                ImGui::DockBuilderDockWindow(/*"Top"*/"Toolbar", dock_id_top);
                ImGui::DockBuilderDockWindow(/*"Bottom"*/"Console", dock_id_bottom);

                ImGui::DockBuilderDockWindow("Left", dock_id_left);
                ImGui::DockBuilderDockWindow(/*"Right"*/"Viewer", dock_id_right);
                
                ImGui::DockBuilderDockWindow("Up", dock_id_up);
                ImGui::DockBuilderDockWindow("Down", dock_id_down);

                ImGui::DockBuilderDockWindow("Center", dock_id_center);

                ImGui::DockBuilderFinish(dockspace_id);
            }
        }

        ImGui::End();

        if (_show_imgui_showcase)
            ImGui::ShowDemoWindow(&_show_imgui_showcase);

        // TODO Should be removed as these windows are temporary and mearly serve to lay out the UI.
        ImGui::Begin("Toolbar");
        ImGui::Text("Hello, top!");
        ImGui::End();

        ImGui::Begin("Viewer");
        ImGui::Text("Hello, right!");
        ImGui::End();

    }

    void DockerStage::Update()
    {

    }

    void DockerStage::OnAttached()
    {

    }

    void DockerStage::OnRemoved()
    {

    }

} // End of namespace ~ DCore
