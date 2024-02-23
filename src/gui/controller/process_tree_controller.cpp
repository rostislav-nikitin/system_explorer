#include "process_tree_controller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Controller
        {
            ProcessTreeController::ProcessTreeController(
                wxBookCtrl *mainBook, SystemExplorer::Core::ProcessManager processManager)
                : ControllerBase(), _processManager(processManager)
            {
                //TODO: Move code here
            }

            ProcessTreeController::~ProcessTreeController()
            {
            
            }
            void ProcessTreeController::CreateChildControls()
            {

            }
            void ProcessTreeController::BindData()
            {

            }

            void ProcessTreeController::BindEvents()
            {

            }

        }
    }
}
