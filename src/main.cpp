#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

class SpectraWindow final : public QMainWindow {
public:
    SpectraWindow() {
        setWindowTitle("Spectra | Homebrew Research Toolkit");
        resize(900, 620);

        auto *root = new QWidget(this);
        auto *layout = new QVBoxLayout(root);
        auto *intro = new QLabel(
            "<h2>Spectra</h2><p>PS5 homebrew file organizer &amp; research UI</p>"
            "<p><b>Simulation mode:</b> this prototype only inspects selections. "
            "It does not inject code, jailbreak a console, or write files to a device.</p>");
        intro->setWordWrap(true);
        layout->addWidget(intro);

        auto *actions = new QHBoxLayout;
        auto *chooseFiles = new QPushButton("Select homebrew files…");
        auto *chooseFolder = new QPushButton("Choose USB / folder…");
        auto *validate = new QPushButton("Validate selection (simulation)");
        actions->addWidget(chooseFiles);
        actions->addWidget(chooseFolder);
        actions->addWidget(validate);
        layout->addLayout(actions);

        layout->addWidget(new QLabel("Selected files"));
        files_ = new QListWidget;
        layout->addWidget(files_, 2);

        destination_ = new QLabel("No destination folder selected");
        destination_->setTextInteractionFlags(Qt::TextSelectableByMouse);
        layout->addWidget(destination_);

        layout->addWidget(new QLabel("Activity log"));
        log_ = new QTextEdit;
        log_->setReadOnly(true);
        layout->addWidget(log_, 2);

        setCentralWidget(root);
        connect(chooseFiles, &QPushButton::clicked, this, [this] { selectFiles(); });
        connect(chooseFolder, &QPushButton::clicked, this, [this] { selectDestination(); });
        connect(validate, &QPushButton::clicked, this, [this] { validateSelection(); });
        logEvent("Spectra started in simulation mode.");
    }

private:
    QListWidget *files_{};
    QTextEdit *log_{};
    QLabel *destination_{};
    QStringList selectedFiles_;
    QString destinationPath_;

    void logEvent(const QString &message) {
        const auto timestamp = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
        log_->append(QString("[%1] %2").arg(timestamp, message.toHtmlEscaped()));
    }

    void selectFiles() {
        const auto paths = QFileDialog::getOpenFileNames(
            this, "Select files to inspect", QDir::homePath(), "All files (*)");
        if (paths.isEmpty()) {
            logEvent("File selection cancelled.");
            return;
        }
        for (const auto &path : paths) {
            if (!selectedFiles_.contains(path)) {
                selectedFiles_.append(path);
                files_->addItem(path);
            }
        }
        logEvent(QString("Selected %1 file(s) for inspection.").arg(paths.size()));
    }

    void selectDestination() {
        const auto path = QFileDialog::getExistingDirectory(
            this, "Choose a folder or mounted USB drive", QDir::homePath());
        if (path.isEmpty()) {
            logEvent("Destination selection cancelled.");
            return;
        }
        destinationPath_ = path;
        destination_->setText("Selected folder: " + path);
        logEvent("Destination folder selected: " + path);
        logEvent("No files have been copied or modified.");
    }

    void validateSelection() {
        int validCount = 0;
        for (const auto &path : selectedFiles_) {
            const QFileInfo info(path);
            if (info.exists() && info.isFile() && info.isReadable()) {
                ++validCount;
            } else {
                logEvent("Warning: file is missing, unreadable, or not a regular file: " + path);
            }
        }
        logEvent(QString("Simulation complete: %1 of %2 selected file(s) are readable.")
                     .arg(validCount).arg(selectedFiles_.size()));
        if (destinationPath_.isEmpty()) {
            logEvent("No destination selected. Choose a folder to record a simulated workflow.");
        } else {
            logEvent("Destination recorded for simulation: " + destinationPath_);
        }
        logEvent("Simulation only: no exploit was run and no device or destination was changed.");
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("Spectra");
    SpectraWindow window;
    window.show();
    return app.exec();
}
