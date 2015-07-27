#ifndef DIALOGVIEWCER_H
#define DIALOGVIEWCER_H

#include <QDialog>

namespace Ui {
class DialogViewCer;
}

class DialogViewCer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogViewCer(QWidget *parent = 0);
    ~DialogViewCer();

private:
    Ui::DialogViewCer *ui;
};

#endif // DIALOGVIEWCER_H
