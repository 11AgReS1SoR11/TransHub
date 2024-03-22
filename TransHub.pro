# Specify that this project is a collection of subprojects
TEMPLATE = subdirs

# Define the directories for the GUI application, service application, and include files
GUI_APP_DIR = GuiApplication
SERVICE_APP_DIR = ServiceApplication
COMPONENTS_CORE = Common/ComponentsCore5alpha
GUI_CORE = Common/ComponentSystemGuiCoreNg
PROCESSING_GUI = Common/ComponentProcessingGui
COMMON_DIR = Common

# Add the subdirectories for the GUI application and service application to SUBDIRS
SUBDIRS += $$COMPONENTS_CORE \
           $$GUI_APP_DIR \
           $$SERVICE_APP_DIR \
           $$GUI_CORE \
           $$PROCESSING_GUI \

