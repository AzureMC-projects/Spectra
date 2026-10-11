#include <QApplication>
#include <QDesktopServices>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QUrl>

class SimulatorWindow final : public QMainWindow {
public:
    SimulatorWindow() {
        setWindowTitle("Spectra Simulator | Console-style desktop");
        resize(1280, 800);
        setMinimumSize(900, 600);
        setStyleSheet(R"(
            QMainWindow, QWidget#page { background: #071326; color: #f4f7ff; }
            QLabel { color: #f4f7ff; }
            QLabel#muted { color: #a9b9d4; }
            QLabel#hero { font-size: 30px; font-weight: 700; }
            QLabel#section { font-size: 19px; font-weight: 600; }
            QPushButton { color: white; background: #173a68; border: 1px solid #4275b7;
                          border-radius: 9px; padding: 11px 16px; }
            QPushButton:hover { background: #24548d; }
            QPushButton#tile { text-align: left; min-width: 150px; min-height: 125px;
                               font-size: 17px; font-weight: 600; background: #10294b; }
            QPushButton#primary { background: #dceaff; color: #092044; border: none; font-weight: 700; }
            QLineEdit { color: white; background: #10213b; border: 1px solid #375579;
                        border-radius: 8px; padding: 12px; }
            QFrame#topbar { background: #0a1b32; border-bottom: 1px solid #25466d; }
        )");

        auto *central = new QWidget;
        auto *outer = new QVBoxLayout(central);
        outer->setContentsMargins(0, 0, 0, 0);
        outer->setSpacing(0);

        auto *topbar = new QFrame;
        topbar->setObjectName("topbar");
        auto *top = new QHBoxLayout(topbar);
        auto *brand = new QLabel("SPECTRA");
        brand->setStyleSheet("font-size: 20px; font-weight: 800; letter-spacing: 3px; color: #91c5ff;");
        top->addWidget(brand);
        top->addStretch();
        auto *homeNav = new QPushButton("Home");
        auto *browserNav = new QPushButton("Browser");
        auto *storeNav = new QPushButton("Store");
        top->addWidget(homeNav);
        top->addWidget(browserNav);
        top->addWidget(storeNav);
        outer->addWidget(topbar);

        pages_ = new QStackedWidget;
        pages_->addWidget(makeHomePage());
        pages_->addWidget(makeBrowserPage());
        pages_->addWidget(makeStorePage());
        outer->addWidget(pages_, 1);
        setCentralWidget(central);

        connect(homeNav, &QPushButton::clicked, this, [this] { pages_->setCurrentIndex(0); });
        connect(browserNav, &QPushButton::clicked, this, [this] { pages_->setCurrentIndex(1); });
        connect(storeNav, &QPushButton::clicked, this, [this] { pages_->setCurrentIndex(2); });
    }

private:
    QStackedWidget *pages_{};

    QWidget *newPage() {
        auto *page = new QWidget;
        page->setObjectName("page");
        auto *layout = new QVBoxLayout(page);
        layout->setContentsMargins(38, 30, 38, 30);
        layout->setSpacing(18);
        return page;
    }

    QWidget *makeHomePage() {
        auto *page = newPage();
        auto *layout = qobject_cast<QVBoxLayout *>(page->layout());
        auto *eyebrow = new QLabel("WELCOME BACK");
        eyebrow->setObjectName("muted");
        layout->addWidget(eyebrow);
        auto *hero = new QLabel("Your space. Your library.");
        hero->setObjectName("hero");
        layout->addWidget(hero);
        auto *subtitle = new QLabel("A console-inspired desktop shell for exploring local web content and store links.");
        subtitle->setObjectName("muted");
        layout->addWidget(subtitle);

        auto *tiles = new QHBoxLayout;
        auto *browser = new QPushButton("◉\n\nBrowser\nOpen web pages");
        browser->setObjectName("tile");
        auto *store = new QPushButton("▣\n\nStore\nExplore games");
        store->setObjectName("tile");
        auto *library = new QPushButton("▤\n\nLocal Library\nChoose a content folder");
        library->setObjectName("tile");
        tiles->addWidget(browser);
        tiles->addWidget(store);
        tiles->addWidget(library);
        layout->addLayout(tiles);
        layout->addSpacing(20);
        auto *note = new QLabel("SIMULATOR MODE  •  Windows desktop\nThis is a console-inspired interface, not a PlayStation 5 emulator. PS5 games and system software do not run here.");
        note->setObjectName("muted");
        note->setWordWrap(true);
        layout->addWidget(note);
        layout->addStretch();

        connect(browser, &QPushButton::clicked, this, [this] { pages_->setCurrentIndex(1); });
        connect(store, &QPushButton::clicked, this, [this] { pages_->setCurrentIndex(2); });
        connect(library, &QPushButton::clicked, this, [this] {
            QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::homePath()));
        });
        return page;
    }

    QWidget *makeBrowserPage() {
        auto *page = newPage();
        auto *layout = qobject_cast<QVBoxLayout *>(page->layout());
        auto *hero = new QLabel("Internet Browser");
        hero->setObjectName("hero");
        layout->addWidget(hero);
        auto *hint = new QLabel("Enter a website address. Spectra opens it in your normal Windows browser; this shell does not embed or emulate the PS5 browser.");
        hint->setObjectName("muted");
        hint->setWordWrap(true);
        layout->addWidget(hint);

        auto *addressRow = new QHBoxLayout;
        auto *address = new QLineEdit("https://www.google.com");
        address->setPlaceholderText("https://example.com");
        auto *open = new QPushButton("Open website");
        open->setObjectName("primary");
        addressRow->addWidget(address, 1);
        addressRow->addWidget(open);
        layout->addLayout(addressRow);

        auto *quickLinks = new QLabel("Quick links");
        quickLinks->setObjectName("section");
        layout->addWidget(quickLinks);
        auto *links = new QHBoxLayout;
        for (const auto &entry : {qMakePair(QString("PlayStation.com"), QString("https://www.playstation.com/")),
                                  qMakePair(QString("PlayStation Store"), QString("https://store.playstation.com/")),
                                  qMakePair(QString("Example page"), QString("https://example.com"))}) {
            auto *button = new QPushButton(entry.first);
            links->addWidget(button);
            connect(button, &QPushButton::clicked, this, [url = entry.second] {
                QDesktopServices::openUrl(QUrl(url));
            });
        }
        layout->addLayout(links);
        layout->addStretch();
        auto openAddress = [address] {
            QString text = address->text().trimmed();
            if (text.isEmpty()) return;
            if (!text.contains("://")) text.prepend("https://");
            const QUrl url(text);
            if (url.isValid() && !url.host().isEmpty())
                QDesktopServices::openUrl(url);
        };
        connect(open, &QPushButton::clicked, this, openAddress);
        connect(address, &QLineEdit::returnPressed, this, openAddress);
        return page;
    }

    QWidget *makeStorePage() {
        auto *page = newPage();
        auto *layout = qobject_cast<QVBoxLayout *>(page->layout());
        auto *hero = new QLabel("Store");
        hero->setObjectName("hero");
        layout->addWidget(hero);
        auto *hint = new QLabel("Storefront shortcuts. These open official web pages; this app does not access private store APIs or install console software.");
        hint->setObjectName("muted");
        hint->setWordWrap(true);
        layout->addWidget(hint);

        auto addStoreCard = [layout](const QString &title, const QString &description,
                                     const QString &buttonText, const QString &url) {
            auto *card = new QFrame;
            card->setStyleSheet("QFrame { background: #102441; border: 1px solid #29496f; border-radius: 12px; }");
            auto *row = new QHBoxLayout(card);
            auto *copy = new QVBoxLayout;
            auto *name = new QLabel(title);
            name->setStyleSheet("font-size: 18px; font-weight: 700;");
            auto *desc = new QLabel(description);
            desc->setObjectName("muted");
            desc->setWordWrap(true);
            copy->addWidget(name);
            copy->addWidget(desc);
            row->addLayout(copy, 1);
            auto *open = new QPushButton(buttonText);
            open->setObjectName("primary");
            row->addWidget(open);
            QObject::connect(open, &QPushButton::clicked, card, [url] {
                QDesktopServices::openUrl(QUrl(url));
            });
            layout->addWidget(card);
        };

        addStoreCard("PlayStation Store", "Browse the official PlayStation digital storefront.", "Open Store", "https://store.playstation.com/");
        addStoreCard("PlayStation Plus", "View subscription information and benefits.", "Learn more", "https://www.playstation.com/ps-plus/");
        addStoreCard("PlayStation Support", "Official help and support resources.", "Get support", "https://www.playstation.com/support/");
        layout->addStretch();
        return page;
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("Spectra Simulator");
    SimulatorWindow window;
    window.show();
    return app.exec();
}
