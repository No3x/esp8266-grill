#include <EmbAJAX.h>

class EmbAJAXStylesheet : public EmbAJAXBase {
public:
    EmbAJAXStylesheet(const char* stylesheet) {
        _stylesheet = stylesheet;
    }
    void print() const override {
        _driver->printContent("<script type=\"text/javascript\">");
        _driver->printContent("document.querySelector('head').innerHTML += '<link rel=\"stylesheet\" href=\"");
        _driver->printContent(_stylesheet);
        _driver->printContent("\" type=\"text/css\"/>';");
        _driver->printContent("</script>");
    }
private:
    const char* _stylesheet;
};

class EmbAJAXViewport : public EmbAJAXBase {
public:
    EmbAJAXViewport() {
    }
    void print() const override {
        _driver->printContent("<script type=\"text/javascript\">");
        _driver->printContent("var viewPortTag=document.createElement('meta');");
        _driver->printContent("viewPortTag.id=\"viewport\";");
        _driver->printContent("viewPortTag.name = \"viewport\";");
        _driver->printContent("viewPortTag.content = \"width=320; initial-scale=1.0; maximum-scale=1.0; user-scalable=0;\";");
        _driver->printContent("document.getElementsByTagName('head')[0].appendChild(viewPortTag);");
        _driver->printContent("</script>");
    }
};
