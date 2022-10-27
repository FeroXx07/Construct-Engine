#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial()
{

}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::PassTextures(vector<Texture> &t)
{
	m_Textures = t;
}

void ComponentMaterial::OnEditor()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	ImGuiInputTextFlags iflags = ImGuiInputTextFlags_ReadOnly;
	if (ImGui::CollapsingHeader("Material Information", flags))
	{
		ImGui::Checkbox("Checker tex", &m_DisplayChecker_Tex);
		ImGui::Text("Material name: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", m_MaterialName.c_str());
		ImGui::Separator();

        for (auto tex : this->m_Textures)
        {
            // Path
            ImGui::Text("Path: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", tex.path.c_str());
            ImGui::Separator();

            // Type
            ImGui::Text("Type: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", tex.type.c_str());
            ImGui::Separator();

            // Format RGBA...
            ImGui::Text("Format: ");
            ImGui::SameLine();
            if (tex.nComponents == 3)
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "GL_RGB");
            else if (tex.nComponents == 4)
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "GL_RGBA");
            ImGui::Separator();

            // Size
            ImGui::Text("Width: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " %u", tex.width);
            ImGui::SameLine();
            ImGui::Text("Height: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), " %u", tex.height);
            ImGui::Separator();

            ImGuiIO& io = ImGui::GetIO();
            ImTextureID my_tex_id = (void*)(intptr_t)tex.id;
            float my_tex_w = (float)tex.width;
            float my_tex_h = (float)tex.height;

            ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
            ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
            ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
            //// Normal size
            //ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
            // Half size, same contents
            ImGui::Image(my_tex_id, ImVec2(my_tex_w*0.25f, my_tex_h * 0.25f), uv_min, uv_max, tint_col, border_col);

        }
        //ImGuiIO& io = ImGui::GetIO();
        //ImTextureID my_tex_id = io.Fonts->TexID;
        //float my_tex_w = (float)io.Fonts->TexWidth;
        //float my_tex_h = (float)io.Fonts->TexHeight;
        //{
        //    ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
        //    ImVec2 pos = ImGui::GetCursorScreenPos();
        //    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
        //    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
        //    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
        //    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

        //    ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);

        //    if (ImGui::IsItemHovered())
        //    {
        //        ImGui::BeginTooltip();
        //        float region_sz = 32.0f;
        //        float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
        //        float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
        //        float zoom = 4.0f;
        //        if (region_x < 0.0f) { region_x = 0.0f; }
        //        else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
        //        if (region_y < 0.0f) { region_y = 0.0f; }
        //        else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
        //        ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
        //        ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
        //        ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
        //        ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
        //        ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
        //        ImGui::EndTooltip();
        //    }
        //}
		/*ImGui::Text("Mesh vertices: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u", m_Mesh->vertices.size());
		ImGui::Separator();

		ImGui::Text("Mesh faces: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u", m_Mesh->totalFaces);
		ImGui::Separator();*/
	}
}
