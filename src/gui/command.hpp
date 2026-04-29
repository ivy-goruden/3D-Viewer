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
    OpenCommand(GuiApp* app) : Command(app) {};
    void execute() override;
};

class SaveCommand : public Command {
  public:
    SaveCommand(GuiApp* app) : Command(app) {};
    void execute() override;
};

class ResetCommand : public Command {
  public:
    ResetCommand(GuiApp* app) : Command(app) {};
    void execute() override;
};

class RotateXCommand : public Command {
  public:
    RotateXCommand(GuiApp* app) : Command(app) {};
    void execute() override;
};

class RotateYCommand : public Command {
  public:
    RotateYCommand(GuiApp* app) : Command(app) {};
    void execute() override;
};

class RotateZCommand : public Command {
  public:
    RotateZCommand(GuiApp* app) : Command(app) {};
    void execute() override;
};

class ShiftCommand : public Command {
  public:
    ShiftCommand(GuiApp* app) : Command(app) {};
    void execute() override;
};

class ZoomCommand : public Command {
  public:
    ZoomCommand(GuiApp* app) : Command(app) {};
    void execute() override;
};

class LineSwitchCommand : public Command {
  public:
    LineSwitchCommand(GuiApp* app) : Command(app) {};
    void execute() override;
};

class ProjSwitchCommand : public Command {
  public:
    ProjSwitchCommand(GuiApp* app) : Command(app) {};
    void execute() override;
};

class FillSwitchCommand : public Command {
  public:
    FillSwitchCommand(GuiApp* app) : Command(app) {};
    void execute() override;
};

class VertModeCommand : public Command {
  public:
    VertModeCommand(GuiApp* app) : Command(app) {};
    void execute() override;
};

class ColorCommand : public Command {
  public:
    ColorCommand(GuiApp* app) : Command(app) {};
    void execute() override;
};

#endif
