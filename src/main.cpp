#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QStorageInfo>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QMessageBox>
#include <QWidget>

class SpectraWindow final : public QMainWindow {
public:
    SpectraWindow() {
        setWindowTitle("Spectra | Homebrew Research Toolkit");
        resize(1000, 720);

        auto *root = new QWidget(this);
        auto *layout = new QVBoxLayout(root);
        auto *intro = new QLabel(
            "<h2>Spectra</h2><p>PS5 homebrew file organizer &amp; research UI</p>"
            "<p><b>Simulation mode only:</b> inspect files, select a destination, and record actions. "
            "This prototype does not inject code, jailbreak a console, copy files, or communicate with a PS5.</p>");
        intro->setWordWrap(true);
        layout->addWidget(intro);

        auto *actions = new QHBoxLayout;
        auto *chooseFiles = new QPushButton("Select files…");
        auto *removeFiles = new QPushButton("Remove selected");
        auto *chooseFolder = new QPushButton("Choose folder…");
        auto *refreshDrives = new QPushButton("Refresh drives");
        actions->addWidget(chooseFiles);
        actions->addWidget(removeFiles);
        actions->addWidget(chooseFolder);
        actions->addWidget(refreshDrives);
        layout->addLayout(actions);

        auto *driveRow = new QHBoxLayout;
        driveRow->addWidget(new QLabel("Mounted volumes:"));
        drives_ = new QComboBox;
        drives_->setMinimumWidth(360);
        auto *useDrive = new QPushButton("Use selected volume");
        driveRow->addWidget(drives_, 1);
        driveRow->addWidget(useDrive);
        layout->addLayout(driveRow);

        layout->addWidget(new QLabel("Selected files for inspection"));
        files_ = new QListWidget;
        files_->setSelectionMode(QAbstractItemView::ExtendedSelection);
        layout->addWidget(files_, 2);

        destination_ = new QLabel("No destination folder selected");
        destination_->setWordWrap(true);
        destination_->setTextInteractionFlags(Qt::TextSelectableByMouse);
        layout->addWidget(destination_);

        auto *workflow = new QHBoxLayout;
        auto *validate = new QPushButton("Validate selection");
        auto *simulate = new QPushButton("Run simulation");
        auto *exportLog = new QPushButton("Export activity log…");
        workflow->addWidget(validate);
        workflow->addWidget(simulate);
        workflow->addWidget(exportLog);
        layout->addLayout(workflow);

        layout->addWidget(new QLabel("Activity log"));
        log_ = new QTextEdit;
        log_->setReadOnly(true);
        layout->addWidget(log_, 2);

        setCentralWidget(root);
        connect(chooseFiles, &QPushButton::clicked, this, [this] { selectFiles(); });
        connect(removeFiles, &QPushButton::clicked, this, [this] { removeSelectedFiles(); });
        connect(chooseFolder, &QPushButton::clicked, this, [this] { selectDestination(); });
        connect(refreshDrives, &QPushButton::clicked, this, [this] { refreshVolumes(); });
        connect(useDrive, &QPushButton::clicked, this, [this] { useSelectedVolume(); });
        connect(validate, &QPushButton::clicked, this, [this] { validateSelection(); });
        connect(simulate, &QPushButton::clicked, this, [this] { runSimulation(); });
        connect(exportLog, &QPushButton::clicked, this, [this] { exportActivityLog(); });

        refreshVolumes();
        logEvent("Spectra started in simulation mode.");
        logEvent("Console injection and jailbreak operations are not implemented.");
    }

private:
    QListWidget *files_{};
    QTextEdit *log_{};
    QLabel *destination_{};
    QComboBox *drives_{};
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
        int added = 0;
        for (const auto &path : paths) {
            if (!selectedFiles_.contains(path)) {
                selectedFiles_.append(path);
                files_->addItem(path);
                ++added;
            }
        }
        logEvent(QString("Added %1 new file(s) for inspection.").arg(added));
    }

    void removeSelectedFiles() {
        const auto selected = files_->selectedItems();
        for (auto *item : selected) {
            selectedFiles_.removeAll(item->text());
            delete item;
        }
        logEvent(QString("Removed %1 selected file entry/entries.").arg(selected.size()));
    }

    void refreshVolumes() {
        drives_->clear();
        const auto volumes = QStorageInfo::mountedVolumes();
        for (const auto &volume : volumes) {
            if (!volume.isValid() || !volume.isReady())
                continue;
            const QString label = QString("%1 — %2 (%3 free)")
                .arg(volume.displayName().isEmpty() ? QString("Mounted volume") : volume.displayName(),
                     volume.rootPath(),
                     QString::number(volume.bytesAvailable()));
            drives_->addItem(label, volume.rootPath());
        }
        logEvent(QString("Mounted-volume list refreshed: %1 available volume(s).").arg(drives_->count()));
    }

    void useSelectedVolume() {
        if (drives_->currentIndex() < 0) {
            logEvent("No mounted volume is available to select.");
            return;
        }
        destinationPath_ = drives_->currentData().toString();
        destination_->setText("Selected destination: " + destinationPath_);
        logEvent("Mounted volume selected for simulation: " + destinationPath_);
        logEvent("Selection only: no files have been copied or modified.");
    }

    void selectDestination() {
        const auto path = QFileDialog::getExistingDirectory(
            this, "Choose a folder or mounted USB drive", QDir::homePath());
        if (path.isEmpty()) {
            logEvent("Destination folder selection cancelled.");
            return;
        }
        destinationPath_ = path;
        destination_->setText("Selected destination: " + path);
        logEvent("Destination folder selected for simulation: " + path);
        logEvent("No files have been copied or modified.");
    }

    int readableFileCount() {
        int validCount = 0;
        for (const auto &path : selectedFiles_) {
            const QFileInfo info(path);
            if (info.exists() && info.isFile() && info.isReadable()) {
                ++validCount;
            } else {
                logEvent("Warning: missing, unreadable, or non-regular file: " + path);
            }
        }
        return validCount;
    }

    void validateSelection() {
        const int validCount = readableFileCount();
        logEvent(QString("Validation result: %1 of %2 selected file(s) are readable regular files.")
                     .arg(validCount).arg(selectedFiles_.size()));
        if (destinationPath_.isEmpty())
            logEvent("No destination selected. Choose a folder or mounted volume to record the simulated workflow.");
        else
            logEvent("Destination recorded: " + destinationPath_);
        logEvent("Validation does not copy, execute, or install any file.");
    }

    void runSimulation() {
        const int validCount = readableFileCount();
        logEvent("----- Simulation report -----");
        logEvent(QString("Files selected: %1; readable files: %2").arg(selectedFiles_.size()).arg(validCount));
        logEvent(destinationPath_.isEmpty()
            ? "Destination: not selected."
            : "Destination: " + destinationPath_);
        logEvent("Result: simulated review only. No file transfer, payload execution, console connection, or jailbreak occurred.");
        QMessageBox::information(this, "Simulation complete",
            QString("Review simulation complete.\nReadable files: %1 / %2\n\nNo files were copied and no console was modified.")
                .arg(validCount).arg(selectedFiles_.size()));
    }

    void exportActivityLog() {
        const auto path = QFileDialog::getSaveFileName(
            this, "Export activity log", QDir::home().filePath("spectra-activity-log.txt"),
            "Text files (*.txt);;All files (*)");
        if (path.isEmpty()) {
            logEvent("Activity-log export cancelled.");
            return;
        }
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            logEvent("Error: could not write activity log to " + path);
            QMessageBox::warning(this, "Export failed", "Could not open the chosen file for writing.");
            return;
        }
        file.write(log_->toPlainText().toUtf8());
        file.close();
        logEvent("Activity log exported to: " + path);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("Spectra");
    SpectraWindow window;
    window.show();
    return app.exec();
}
