# Specify that this project is a collection of subprojects
TEMPLATE = subdirs

# Define the directories for the GUI application, service application, and include files
GUI_APP_DIR = GuiApplication
SERVICE_APP_DIR = ServiceApplication
COMMON_DIR = Common

# Add the subdirectories for the GUI application and service application to SUBDIRS
SUBDIRS += $$COMMON_DIR \
           $$GUI_APP_DIR \
           $$SERVICE_APP_DIR

