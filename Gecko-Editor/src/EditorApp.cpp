#define GK_INCLUDE_ENTRYPOINT
#include "Gecko.h"

#include "Layer.h"

namespace Gecko
{
    class GeckoEditor : public Gecko::Application
    {
    public:
        GeckoEditor()
            : Application("Gecko Editor")
        {
            PushLayer(new EditorLayer());
        }
        ~GeckoEditor()
        {
        }
    };
    Gecko::Application *Gecko::CreateApplication()
    {
        return new GeckoEditor();
    }

} // namespace Gecko