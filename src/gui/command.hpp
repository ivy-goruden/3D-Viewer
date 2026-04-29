#ifndef COMMAND_APP
#define COMMAND_APP

class GuiApp;

class Command {
  public:
    GuiApp* app;
    Command(GuiApp* app) : app(app) {};
    virtual ~Command() {};
    virtual void execute() = 0;
};

class OpenCommand : public Command {
  public:
    OpenCommand(GuiApp app) : Command(app) {};
    void execute() override;
};

class SaveCommand : public Command {
  public:
    SaveCommand(GuiApp app) : Command(app) {};
    void execute() override;
};

class ResetCommand : public Command {
  public:
    ResetCommand(GuiApp app) : Command(app) {};
    void execute() override;
};

class RorateXCommand : public Command {
  public:
    RotateXCommand(GuiApp app) : Command(app) {};
    void execute() override;
};

class ShiftCommand : public Command {
  public:
    ShiftCommand(GuiApp app) : Command(app) {};
    void execute() override;
};

class ZoomCommand : public Command {
  public:
    ZoomCommand(GuiApp app) : Command(app) {};
    void execute() override;
};

#endif
