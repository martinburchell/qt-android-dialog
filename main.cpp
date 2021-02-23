#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QLayout>
#include <QPointer>
#include <QScreen>
#include <QVBoxLayout>

class TestDialog : public QDialog
{
    Q_OBJECT
public:
    TestDialog(QWidget* parent) :  QDialog(parent)
    {
        m_prompt = new QLabel();
        m_prompt->setWordWrap(true);

        auto mainlayout = new QVBoxLayout();
        mainlayout->addWidget(m_prompt);
        mainlayout->addStretch(1);

        setLayout(mainlayout);

        QScreen *screen = QGuiApplication::primaryScreen();
        screen->setOrientationUpdateMask(
            Qt::LandscapeOrientation |
            Qt::PortraitOrientation |
            Qt::InvertedLandscapeOrientation |
            Qt::InvertedPortraitOrientation
            );

        connect(screen, &QScreen::orientationChanged,
                this, &TestDialog::orientationChanged);

        orientationChanged(screen->orientation());
    }

public slots:
    void orientationChanged(Qt::ScreenOrientation orientation)
    {
        QString description;

        switch (orientation) {

        case Qt::LandscapeOrientation:
            description = "landscape";
            break;

        case Qt::PortraitOrientation:
            description = "portrait";
            break;

        case Qt::InvertedLandscapeOrientation:
            description = "inverted landscape";
            break;

        case Qt::InvertedPortraitOrientation:
            description = "inverted portrait";
            break;

        default:
            description = "unknown";
            break;
        }

        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screen_rect = screen->geometry();

        QString label = QString("Orientation:%1\nScreen:%2x%3\nDialog:%4x%5\nPos:%6,%7\n").
            arg(description).arg(screen_rect.width()).arg(screen_rect.height()).arg(width()).arg(height()).arg(pos().x()).arg(pos().y());
        m_prompt->setText(label);
    }

protected:
    QPointer<QLabel> m_prompt;
};

#if defined __clang__  // NB defined in Qt Creator; put this first for that reason
    #define COMPILER_IS_CLANG
    #if __clang_major__ >= 10
        #define CLANG_AT_LEAST_10
    #endif
#elif defined __GNUC__  // __GNUC__ is defined for GCC and clang
    #define COMPILER_IS_GCC
    #if __GNUC__ >= 7  // gcc >= 7.0
        #define GCC_AT_LEAST_7
    #endif
#elif defined _MSC_VER
    #define COMPILER_IS_VISUAL_CPP
#endif


#if defined COMPILER_IS_CLANG || defined COMPILER_IS_GCC
    #define VISIBLE_SYMBOL __attribute__ ((visibility ("default")))
#elif defined COMPILER_IS_VISUAL_CPP
    #define VISIBLE_SYMBOL __declspec(dllexport)
#else
    #error "Don't know how to enforce symbol visibility for this compiler!"
#endif


VISIBLE_SYMBOL int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    TestDialog dialog(nullptr);
    dialog.exec();

    return app.exec();
}

#include "main.moc"
