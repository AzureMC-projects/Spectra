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
#include <QProgressBar>
#include <QWidget>

class SpectraWindow final : public QMainWindow {
public:
    SpectraWindow() {
        setWindowTitle("Spectra | File Copier");
        resize(1000, 720);

        auto *root = new QWidget(this);
        auto *layout = new QVBoxLayout(root);
        auto *intro = new QLabel(
            "<h2>Spectra</h2><p>Choose files and copy them to a folder you control.</p>"
            "<p>Copies are ordinary filesystem operations on this computer or a mounted drive. "
            "Spectra does not connect to, jailbreak, or inject code into a console.</p>");
        intro->setWordWrap(true);
        layout->addWidget(intro);

        auto *actions = new QHBoxLayout;
        auto *chooseFiles = new QPushButton("Select files…");
        auto *removeFiles = new QPushButton("Remove selected");
        auto *chooseFolder = new QPushButton("Choose destination…");
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

        layout->addWidget(new QLabel("Files to copy"));
        files_ = new QListWidget;
        files_->setSelectionMode(QAbstractItemView::ExtendedSelection);
        layout->addWidget(files_, 2);

        destination_ = new QLabel("No destination folder selected");
        destination_->setWordWrap(true);
        destination_->setTextInteractionFlags(Qt::TextSelectableByMouse);
        layout->addWidget(destination_);

        auto *workflow = new QHBoxLayout;
        auto *validate = new QPushButton("Validate selection");
        auto *copy = new QPushButton("Copy files");
        auto *exportLog = new QPushButton("Export activity log…");
        workflow->addWidget(validate);
        workflow->addWidget(copy);
        workflow->addWidget(exportLog);
        layout->addLayout(workflow);

        progress_ = new QProgressBar;
        progress_->setRange(0, 100);
        progress_->setValue(0);
        progress_->setFormat("%p%");
        layout->addWidget(progress_);

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
        connect(copy, &QPushButton::clicked, this, [this] { copyFiles(); });
        connect(exportLog, &QPushButton::clicked, this, [this] { exportActivityLog(); });

        refreshVolumes();
        logEvent("Spectra started. File copying is limited to the selected destination folder.");
    }

private:
    QListWidget *files_{};
    QTextEdit *log_{};
    QLabel *destination_{};
    QComboBox *drives_{};
    QProgressBar *progress_{};
    QStringList selectedFiles_;
    QString destinationPath_;

    void logEvent(const QString &message) {
        const auto timestamp = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
        log_->append(QString("[%1] %2").arg(timestamp, message.toHtmlEscaped()));
    }

    void selectFiles() {
        const auto paths = QFileDialog::getOpenFileNames(
            this, "Select files to copy", QDir::homePath(), "All files (*)");
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
        logEvent(QString("Added %1 new file(s).").arg(added));
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
            const QString label = QString("%1 — %2 (%3 bytes free)")
                .arg(volume.displayName().isEmpty() ? QString("Mounted volume") : volume.displayName(),
                     volume.rootPath(),
                     QString::number(volume.bytesAvailable()));
            drives_->addItem(label, volume.rootPath());
        }
        logEvent(QString("Mounted-volume list refreshed: %1 available volume(s).").arg(drives_->count()));
    }

    void setDestination(const QString &path) {
        destinationPath_ = QDir::cleanPath(path);
        destination_->setText("Selected destination: " + destinationPath_);
        logEvent("Destination selected: " + destinationPath_);
    }

    void useSelectedVolume() {
        if (drives_->currentIndex() < 0) {
            logEvent("No mounted volume is available to select.");
            return;
        }
        setDestination(drives_->currentData().toString());
    }

    void selectDestination() {
        const auto path = QFileDialog::getExistingDirectory(
            this, "Choose destination folder", destinationPath_.isEmpty() ? QDir::homePath() : destinationPath_);
        if (path.isEmpty()) {
            logEvent("Destination folder selection cancelled.");
            return;
        }
        setDestination(path);
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
        if (destinationPath_.isEmpty()) {
            logEvent("Choose a destination folder before copying.");
        } else {
            const QFileInfo destInfo(destinationPath_);
            logEvent(destInfo.isDir() && destInfo.isWritable()
                ? "Destination exists and is writable."
                : "Warning: destination is missing, is not a folder, or is not writable.");
        }
    }

    void copyFiles() {
        if (selectedFiles_.isEmpty()) {
            QMessageBox::information(this, "Nothing to copy", "Select one or more files first.");
            return;
        }
        if (destinationPath_.isEmpty()) {
            QMessageBox::warning(this, "Destination required", "Choose a destination folder first.");
            return;
        }

        const QFileInfo destInfo(destinationPath_);
        if (!destInfo.exists() || !destInfo.isDir() || !destInfo.isWritable()) {
            QMessageBox::warning(this, "Invalid destination",
                "The destination must be an existing, writable folder.");
            logEvent("Copy cancelled: destination is missing or not writable.");
            return;
        }

        const int answer = QMessageBox::question(
            this, "Confirm file copy",
            QString("Copy %1 selected file(s) into:\n%2\n\nExisting files may prompt before being replaced.")
                .arg(selectedFiles_.size()).arg(destinationPath_),
            QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
        if (answer != QMessageBox::Yes) {
            logEvent("Copy cancelled by user.");
            return;
        }

        int copied = 0;
        int skipped = 0;
        int failed = 0;
        progress_->setRange(0, selectedFiles_.size());
        progress_->setValue(0);

        for (int i = 0; i < selectedFiles_.size(); ++i) {
            const QString sourcePath = selectedFiles_.at(i);
            const QFileInfo sourceInfo(sourcePath);
            if (!sourceInfo.exists() || !sourceInfo.isFile() || !sourceInfo.isReadable()) {
                logEvent("Skipped invalid or unreadable source: " + sourcePath);
                ++skipped;
                progress_->setValue(i + 1);
                QApplication::processEvents();
                continue;
            }

            const QString targetPath = QDir(destinationPath_).filePath(sourceInfo.fileName());
            const QString canonicalSource = sourceInfo.canonicalFilePath();
            const QString canonicalTarget = QFileInfo(targetPath).canonicalFilePath();
            if (!canonicalSource.isEmpty() && canonicalSource == canonicalTarget) {
                logEvent("Skipped file because source and destination are the same: " + sourcePath);
                ++skipped;
                progress_->setValue(i + 1);
                QApplication::processEvents();
                continue;
            }

            if (QFileInfo::exists(targetPath)) {
                const auto overwrite = QMessageBox::question(
                    this, "File already exists",
                    QString("A file named %1 already exists in the destination. Replace it?")
                        .arg(sourceInfo.fileName()),
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
                if (overwrite == QMessageBox::Cancel) {
                    logEvent("Copy stopped by user.");
                    skipped += selectedFiles_.size() - i;
                    break;
                }
                if (overwrite != QMessageBox::Yes) {
                    logEvent("Skipped existing destination file: " + targetPath);
                    ++skipped;
                    progress_->setValue(i + 1);
                    QApplication::processEvents();
                    continue;
                }
                if (!QFile::remove(targetPath)) {
                    logEvent("Failed to replace existing file (could not remove it): " + targetPath);
                    ++failed;
                    progress_->setValue(i + 1);
                    QApplication::processEvents();
                    continue;
                }
            }

            if (QFile::copy(sourcePath, targetPath)) {
                logEvent("Copied: " + sourcePath + " → " + targetPath);
                ++copied;
            } else {
                logEvent("Copy failed: " + sourcePath + " → " + targetPath
                         + " (" + QFile(sourcePath).errorString() + ")");
                ++failed;
            }
            progress_->setValue(i + 1);
            QApplication::processEvents();
        }

        logEvent(QString("Copy finished. Copied: %1; skipped: %2; failed: %3.")
                     .arg(copied).arg(skipped).arg(failed));
        QMessageBox::information(this, "Copy complete",
            QString("Copied: %1\nSkipped: %2\nFailed: %3\n\nDestination:\n%4")
                .arg(copied).arg(skipped).arg(failed).arg(destinationPath_));
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
