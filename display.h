#ifndef DISPLAY_H_
#define DISPLAY_H_

class Observer {
public:
    virtual void notify() = 0;
};

class TextDisplay : public Observer {
public:
    void notify() override;
};

// TODO Graphical Display

#endif // DISPLAY_H_
