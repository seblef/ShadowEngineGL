
namespace Editor
{

class IWindow
{
protected:
    bool _open;

public:
    IWindow();
    virtual ~IWindow() {};

    bool isOpen() const { return _open; }
    void close() { _open = false; }

    virtual void draw()=0;
};

}