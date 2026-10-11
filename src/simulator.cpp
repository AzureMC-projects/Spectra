#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QDateTime>
#include <QUrl>
#include <QSettings>
#include <QHash>
#include <QTextBrowser>
#include <QScrollArea>
#include <QComboBox>
#include <QFrame>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>

class Simulator final:public QMainWindow{
public:
 Simulator(){setWindowTitle("Spectra Simulator");resize(1360,850);setMinimumSize(980,650);docs_=QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);imports_=QDir(docs_).filePath("Spectra Imports");library_=QDir(docs_).filePath("Spectra Library");QDir().mkpath(imports_);QDir().mkpath(library_);QDir().mkpath(QDir(library_).filePath("v2.1"));settings_=new QSettings("Spectra","Simulator",this);
 setStyleSheet("QMainWindow,QWidget#page{background:#080d18;color:#f5f7ff} QLabel{color:#f5f7ff} QLabel#hero{font-size:30px;font-weight:800} QLabel#muted{color:#9eabc3} QFrame#top{background:#101a2b;border-bottom:1px solid #2b3c58} QPushButton{color:white;background:#16243a;border:1px solid #344c70;border-radius:9px;padding:10px 15px} QPushButton:hover{background:#24466f;border-color:#82c8ff} QPushButton#tile{text-align:left;min-width:160px;min-height:110px;font-size:17px;font-weight:700;background:#111f34} QPushButton#primary{background:#d9ecff;color:#07162a;font-weight:800} QLineEdit,QListWidget,QTextBrowser{color:#f5f7ff;background:#0d1828;border:1px solid #2b3c58;padding:8px}");
 auto *c=new QWidget;auto *v=new QVBoxLayout(c);v->setContentsMargins(0,0,0,0);auto *top=new QWidget;top->setObjectName("top");auto *th=new QHBoxLayout(top);auto *brand=new QLabel("S P E C T R A");brand->setStyleSheet("font-size:21px;font-weight:900;color:#8dcaff");th->addWidget(brand);th->addStretch();th->addWidget(new QLabel(QDateTime::currentDateTime().toString("ddd d MMM  h:mm AP")));v->addWidget(top);
 auto *nav=new QHBoxLayout;QStringList names={"Home","Library","Browser","Store","Media Gallery","Settings"};for(int i=0;i<names.size();++i){auto *b=new QPushButton(names[i]);nav->addWidget(b);connect(b,&QPushButton::clicked,this,[this,i]{showPage(i);});}auto *nw=new QWidget;nw->setLayout(nav);v->addWidget(nw);
 pages_=new QStackedWidget;pages_->addWidget(home());pages_->addWidget(libraryPage());pages_->addWidget(browserPage());pages_->addWidget(storePage());pages_->addWidget(galleryPage());pages_->addWidget(settingsPage());v->addWidget(pages_,1);setCentralWidget(c);showPage(0);}
private:
 QString docs_,imports_,library_;QSettings *settings_{};QStackedWidget *pages_{};QListWidget *lib_{},*gallery_{};QLabel *preview_{};QLineEdit *address_{};
 QWidget* page(){auto*p=new QWidget;p->setObjectName("page");auto*l=new QVBoxLayout(p);l->setContentsMargins(34,26,34,26);l->setSpacing(15);return p;}
 void showPage(int i){pages_->setCurrentIndex(i);if(i==1)refreshLib();if(i==4)refreshGallery();
 }
 void addTile(QGridLayout*g,QString text,int pageIndex){auto*b=new QPushButton(text);b->setObjectName("tile");g->addWidget(b);connect(b,&QPushButton::clicked,this,[this,pageIndex]{showPage(pageIndex);});}
 QWidget*home(){auto*p=page();auto*l=qobject_cast<QVBoxLayout*>(p->layout());auto*ey=new QLabel("PLAYSTATION-INSPIRED HOME");ey->setObjectName("muted");l->addWidget(ey);auto*h=new QLabel("What's new");h->setObjectName("hero");l->addWidget(h);auto*s=new QLabel("Your games, apps and media — arranged like a console home screen.");s->setObjectName("muted");l->addWidget(s);
 auto*g=new QGridLayout;g->setSpacing(18);
 auto makeApp=[this,g](QString icon,QString name,QString detail,int idx,int col){auto*card=new QWidget;card->setStyleSheet("QWidget{background:#111f34;border:1px solid #2b4568;border-radius:15px}");auto*cl=new QVBoxLayout(card);cl->setContentsMargins(15,15,15,15);auto*glyph=new QLabel(icon);glyph->setAlignment(Qt::AlignCenter);glyph->setMinimumHeight(82);glyph->setStyleSheet("font-size:42px;font-weight:900;color:#8dcaff;background:#1a3150;border:1px solid #34547a;border-radius:12px");cl->addWidget(glyph);auto*label=new QLabel(name);label->setStyleSheet("font-size:17px;font-weight:800;border:0;background:transparent");cl->addWidget(label);auto*desc=new QLabel(detail);desc->setWordWrap(true);desc->setObjectName("muted");desc->setStyleSheet("border:0;background:transparent");cl->addWidget(desc);auto*b=new QPushButton("Open");b->setObjectName("primary");cl->addWidget(b);g->addWidget(card,0,col);connect(b,&QPushButton::clicked,this,[this,idx]{showPage(idx);});};
 makeApp("▦","Game Library","Recognised files and supported formats",1,0);
 makeApp("◎","PS Browser","Search and open websites",2,1);
 makeApp("▣","PlayStation Store","Store, Plus and support",3,2);
 makeApp("♫","Media Gallery","Photos, video and audio",4,0);
 makeApp("⚙","Settings","Folders and preferences",5,1);
 auto*importsCard=new QWidget;importsCard->setStyleSheet("background:#111f34;border:1px solid #2b4568;border-radius:15px");auto*il=new QVBoxLayout(importsCard);auto*ii=new QLabel("＋");ii->setAlignment(Qt::AlignCenter);ii->setMinimumHeight(82);ii->setStyleSheet("font-size:42px;color:#8dcaff;background:#1a3150;border:1px solid #34547a;border-radius:12px");il->addWidget(ii);auto*it=new QLabel("Import folder");it->setStyleSheet("font-size:17px;font-weight:800;border:0;background:transparent");il->addWidget(it);auto*id=new QLabel("Drop or copy files here; Spectra detects their type automatically.");id->setWordWrap(true);id->setObjectName("muted");id->setStyleSheet("border:0;background:transparent");il->addWidget(id);auto*ib=new QPushButton("Open imports folder");il->addWidget(ib);g->addWidget(importsCard,0,2);connect(ib,&QPushButton::clicked,this,[this]{QDesktopServices::openUrl(QUrl::fromLocalFile(imports_));});
 l->addLayout(g);l->addStretch();auto*f=new QLabel("SPECTRA • Console-inspired desktop shell • Not a PlayStation emulator");f->setObjectName("muted");l->addWidget(f);return p;}
 QWidget*libraryPage(){auto*p=page();auto*l=qobject_cast<QVBoxLayout*>(p->layout());auto*h=new QLabel("Game Library");h->setObjectName("hero");l->addWidget(h);auto*d=new QLabel("Spectra scans the library and identifies files by extension. Supported media/documents open with their Windows app. Game packages and executable files are labelled, not injected or executed.");d->setObjectName("muted");d->setWordWrap(true);l->addWidget(d);auto*r=new QHBoxLayout;auto*ref=new QPushButton("Rescan library");auto*folder=new QPushButton("Open library folder");r->addWidget(ref);r->addWidget(folder);r->addStretch();l->addLayout(r);lib_=new QListWidget;l->addWidget(lib_,1);auto*open=new QPushButton("Open selected supported file");open->setObjectName("primary");l->addWidget(open);connect(ref,&QPushButton::clicked,this,[this]{refreshLib();});connect(folder,&QPushButton::clicked,this,[this]{QDesktopServices::openUrl(QUrl::fromLocalFile(library_));});connect(open,&QPushButton::clicked,this,[this]{auto*i=lib_->currentItem();if(!i)return;QString path=i->data(Qt::UserRole).toString();QString ext=QFileInfo(path).suffix().toLower();if(!QStringList({"txt","pdf","png","jpg","jpeg","gif","bmp","webp","html","htm","mp4","mp3","wav","m4v","mov"}).contains(ext)){QMessageBox::information(this,"Not an openable document/media file","Spectra detected this file type, but cannot safely inject or run it. Game packages require a compatible emulator or console and are not executed by this desktop shell.");return;}QDesktopServices::openUrl(QUrl::fromLocalFile(path));});refreshLib();return p;}
 void refreshLib(){if(!lib_)return;lib_->clear();QHash<QString,QString> types={{"iso","DISC IMAGE / GAME PACKAGE"},{"pkg","PLAYSTATION PACKAGE"},{"elf","CONSOLE EXECUTABLE"},{"bin","BINARY / DISC DATA"},{"cue","DISC TRACK SHEET"},{"txt","TEXT DOCUMENT"},{"pdf","PDF DOCUMENT"},{"png","IMAGE"},{"jpg","IMAGE"},{"jpeg","IMAGE"},{"gif","IMAGE"},{"bmp","IMAGE"},{"webp","IMAGE"},{"mp4","VIDEO"},{"m4v","VIDEO"},{"mov","VIDEO"},{"avi","VIDEO"},{"mp3","AUDIO"},{"wav","AUDIO"},{"html","WEB DOCUMENT"},{"htm","WEB DOCUMENT"}};QStringList roots={imports_,library_};for(const QString &root:roots){QDirIterator it(root,QDir::Files|QDir::NoDotAndDotDot,QDirIterator::Subdirectories);while(it.hasNext()){QFileInfo f(it.next());QString ext=f.suffix().toLower();QString kind=types.value(ext,ext.isEmpty()?"UNKNOWN FILE":"FILE TYPE: "+ext.toUpper());QString icon=(kind.contains("IMAGE")?"▧":kind.contains("VIDEO")?"▶":kind.contains("AUDIO")?"♫":kind.contains("GAME")||kind.contains("PACKAGE")||kind.contains("DISC")?"🎮":kind.contains("DOCUMENT")||kind.contains("PDF")?"▤":"◇");QString origin=root==imports_?"IMPORTS • ":"LIBRARY • "+QDir(library_).relativeFilePath(f.absolutePath())+" • ";auto*i=new QListWidgetItem(icon+"   "+f.fileName()+"   —   "+origin+kind+"   ("+QString::number(f.size())+" bytes)",lib_);i->setData(Qt::UserRole,f.absoluteFilePath());i->setToolTip("Automatically detected as: "+kind+"\\nLocation: "+f.absoluteFilePath());}}}
 QWidget*browserPage(){auto*p=page();auto*l=qobject_cast<QVBoxLayout*>(p->layout());auto*h=new QLabel("PS BROWSER");h->setObjectName("hero");l->addWidget(h);auto*subtitle=new QLabel("A PlayStation-inspired browser launcher");subtitle->setObjectName("muted");l->addWidget(subtitle);auto*r=new QHBoxLayout;address_=new QLineEdit("https://www.google.com");address_->setPlaceholderText("Enter a website or search");auto*go=new QPushButton("OPEN IN BROWSER");go->setObjectName("primary");r->addWidget(address_,1);r->addWidget(go);l->addLayout(r);auto*panel=new QWidget;panel->setStyleSheet("background:#101a2b;border:1px solid #2b3c58;border-radius:14px");auto*pl=new QVBoxLayout(panel);auto*title=new QLabel("Ready to browse");title->setStyleSheet("font-size:22px;font-weight:800");pl->addWidget(title);auto*note=new QLabel("Choose a shortcut or enter a web address. Websites open in your default browser; this Spectra screen keeps the PS-inspired controls and shortcuts without requiring Qt WebEngine.");note->setWordWrap(true);note->setObjectName("muted");pl->addWidget(note);auto*shortcuts=new QGridLayout;QList<QPair<QString,QString>> sites={{"Google","https://www.google.com"},{"YouTube","https://www.youtube.com"},{"Twitch","https://www.twitch.tv"},{"PlayStation","https://www.playstation.com"}};for(int i=0;i<sites.size();++i){auto*b=new QPushButton(sites[i].first+"  ↗");b->setObjectName("tile");shortcuts->addWidget(b,i/2,i%2);connect(b,&QPushButton::clicked,this,[this,url=sites[i].second]{address_->setText(url);QDesktopServices::openUrl(QUrl(url));});}pl->addLayout(shortcuts);l->addWidget(panel,1);auto navigate=[this]{QString s=address_->text().trimmed();if(s.isEmpty())return;if(!s.contains("://"))s=(s.contains('.')&&!s.contains(' '))?"https://"+s:"https://www.google.com/search?q="+QString(QUrl::toPercentEncoding(s));address_->setText(s);QDesktopServices::openUrl(QUrl(s));};connect(go,&QPushButton::clicked,this,navigate);connect(address_,&QLineEdit::returnPressed,this,navigate);return p;}
 QWidget*storePage(){
  auto*p=page();auto*l=qobject_cast<QVBoxLayout*>(p->layout());
  auto*eyebrow=new QLabel("SPECTRA SIMULATION STORE");eyebrow->setObjectName("muted");l->addWidget(eyebrow);
  auto*h=new QLabel("Discover your next simulation");h->setObjectName("hero");l->addWidget(h);
  auto*sub=new QLabel("A local catalogue of demo apps for the Spectra Simulator. Installations add catalogue files to your selected Spectra version folder; they do not download commercial games or run console packages.");
  sub->setWordWrap(true);sub->setObjectName("muted");l->addWidget(sub);
  auto*toolbar=new QHBoxLayout;auto*search=new QLineEdit;search->setPlaceholderText("Search simulations…");
  auto*category=new QComboBox;category->addItems({"All categories","Arcade","Driving","Sandbox","Space","Tools"});
  auto*importer=new QPushButton("Open Store Importer");toolbar->addWidget(search,1);toolbar->addWidget(category);toolbar->addWidget(importer);l->addLayout(toolbar);
  auto*scroll=new QScrollArea;scroll->setWidgetResizable(true);scroll->setFrameShape(QFrame::NoFrame);
  auto*content=new QWidget;auto*grid=new QGridLayout(content);grid->setSpacing(16);grid->setContentsMargins(4,4,4,4);
  struct Item{QString title,category,tagline,description,icon,color;};
  const QList<Item> items={
   {"Neon Drift","Driving","Night roads. Endless momentum.","A fictional driving-simulation listing for the Spectra Store demo catalogue.","◈","#57c7ff"},
   {"Orbit Foundry","Space","Build beyond the horizon.","A space-themed sandbox concept listing with a local demo manifest.","◎","#b8a2ff"},
   {"Pixel Arena","Arcade","Quick rounds. Big energy.","A fictional arcade concept used to demonstrate store browsing and installation.","✦","#ff82c8"},
   {"Blockscape","Sandbox","Make a world of your own.","A sandbox concept listing. This demo installs catalogue metadata only.","▦","#80e6b0"},
   {"Sim Tools","Tools","Utilities for your library.","A placeholder tools bundle for testing version-folder import workflows.","⚙","#ffd27a"},
   {"Deep Signal","Space","A quiet mystery in deep space.","A fictional exploration concept for the Spectra simulation catalogue.","⌁","#8de2d6"}
  };
  auto*status=new QLabel("Select a title to view details or install its demo listing.");status->setObjectName("muted");l->addWidget(status);
  auto*detail=new QTextBrowser;detail->setMaximumHeight(118);detail->setOpenExternalLinks(false);detail->setText("Choose a simulation above to see its details.");l->addWidget(detail);
  auto*actions=new QHBoxLayout;auto*install=new QPushButton("Install selected listing");install->setObjectName("primary");auto*openLibrary=new QPushButton("Open version folder");actions->addWidget(install);actions->addWidget(openLibrary);actions->addStretch();l->addLayout(actions);
  scroll->setWidget(content);l->addWidget(scroll,1);
  auto*selected=new QString;auto*selectedTitle=new QString;
  QList<QPushButton*> cards;
  for(int i=0;i<items.size();++i){
   const Item item=items[i];auto*card=new QPushButton;card->setObjectName("tile");card->setMinimumHeight(150);card->setStyleSheet(QString("QPushButton#tile{text-align:left;min-width:210px;min-height:150px;padding:14px;background:#111f34;border:1px solid #2b4568;border-radius:14px;} QPushButton#tile:hover{border:1px solid %1;background:#172b45;}").arg(item.color));
   card->setText(item.icon+"   "+item.title+"\n\n"+item.category+"  •  DEMO LISTING\n"+item.tagline);
   grid->addWidget(card,i/3,i%3);cards<<card;
   connect(card,&QPushButton::clicked,this,[detail,status,selected,selectedTitle,item]{
    *selected=item.title;*selectedTitle=item.title;status->setText("Selected: "+item.title+"  •  "+item.category);
    detail->setText("<h3>"+item.title.toHtmlEscaped()+"</h3><p><b>"+item.category.toHtmlEscaped()+"</b> — "+item.tagline.toHtmlEscaped()+"</p><p>"+item.description.toHtmlEscaped()+"</p>");
   });
  }
  auto filter=[cards,items,search,category]{
   const QString q=search->text().trimmed().toLower();const QString cat=category->currentText();
   for(int i=0;i<cards.size();++i){const auto item=items[i];const bool matchesText=q.isEmpty()||item.title.toLower().contains(q)||item.tagline.toLower().contains(q)||item.description.toLower().contains(q);const bool matchesCat=cat=="All categories"||item.category==cat;cards[i]->setVisible(matchesText&&matchesCat);}
  };
  connect(search,&QLineEdit::textChanged,this,[filter](const QString&){filter();});
  connect(category,&QComboBox::currentTextChanged,this,[filter](const QString&){filter();});
  connect(openLibrary,&QPushButton::clicked,this,[this]{QDesktopServices::openUrl(QUrl::fromLocalFile(QDir(library_).filePath("v2.1")));});
  connect(importer,&QPushButton::clicked,this,[this]{
   const QString source=QFileDialog::getExistingDirectory(this,"Select the folder containing a Spectra Store package");
   if(source.isEmpty())return;
   if(!QFileInfo(QDir(source).filePath("store-manifest.json")).isFile()){QMessageBox::warning(this,"Not a Store package","The selected folder must contain store-manifest.json. Choose an exported Spectra Store package folder.");return;}
   const QString dest=QFileDialog::getExistingDirectory(this,"Select the Spectra version folder",QDir(library_).filePath("v2.1"));
   if(dest.isEmpty())return;
   const QString target=QDir(dest).filePath("Spectra Game Store");
   if(QFileInfo(target).exists()){QMessageBox::warning(this,"Already installed","A Spectra Game Store folder already exists there. Move or rename it before importing to avoid overwriting files.");return;}
   if(!QDir().mkpath(target)){QMessageBox::critical(this,"Import failed","Could not create the destination folder.");return;}
   QDirIterator it(source,QDir::Files|QDir::NoDotAndDotDot,QDirIterator::Subdirectories);int copied=0,failed=0;
   while(it.hasNext()){const QString from=it.next();const QString rel=QDir(source).relativeFilePath(from);const QString to=QDir(target).filePath(rel);QDir().mkpath(QFileInfo(to).absolutePath());if(QFile::copy(from,to))++copied;else ++failed;}
   if(failed){QMessageBox::warning(this,"Import finished with errors",QString("Copied %1 file(s); %2 failed.\nDestination: %3").arg(copied).arg(failed).arg(target));}
   else QMessageBox::information(this,"Store imported",QString("Copied %1 file(s) into:\n%2\n\nRestart or rescan the library to see the imported package.").arg(copied).arg(target));
  });
  connect(install,&QPushButton::clicked,this,[this,selected,selectedTitle,status]{
   if(selected->isEmpty()){QMessageBox::information(this,"Choose a title","Select a simulation card first.");return;}
   const QString base=QDir(library_).filePath("v2.1/Store/"+selected->toLower().replace(' ','-'));
   if(!QDir().mkpath(base)){QMessageBox::warning(this,"Install failed","Could not create the listing folder.");return;}
   QFile manifest(QDir(base).filePath("store-item.json"));
   if(!manifest.open(QIODevice::WriteOnly|QIODevice::Truncate)){QMessageBox::warning(this,"Install failed","Could not write the listing manifest.");return;}
   QJsonObject obj;obj["name"]=*selectedTitle;obj["type"]="Spectra Store demo listing";obj["installedAt"]=QDateTime::currentDateTimeUtc().toString(Qt::ISODate);obj["note"]="Catalogue metadata only; no game binaries are included or executed.";
   manifest.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));manifest.close();
   QFile readme(QDir(base).filePath("README.txt"));if(readme.open(QIODevice::WriteOnly|QIODevice::Truncate)){readme.write(((*selectedTitle)+" is a Spectra Simulation Store demo listing.\nThis install contains metadata only, not a playable game.\n").toUtf8());readme.close();}
   status->setText("Installed demo listing: "+*selectedTitle+" → "+base);
   QMessageBox::information(this,"Installed",*selectedTitle+" was added to your local Spectra version folder.\n\nThis is a demo listing, not a game download.");
  });
  return p;
 }
 QWidget*galleryPage(){auto*p=page();auto*l=qobject_cast<QVBoxLayout*>(p->layout());auto*h=new QLabel("Media Gallery");h->setObjectName("hero");l->addWidget(h);auto*row=new QHBoxLayout;auto*ref=new QPushButton("Refresh");auto*folder=new QPushButton("Open library folder");row->addWidget(ref);row->addWidget(folder);row->addStretch();l->addLayout(row);auto*body=new QHBoxLayout;gallery_=new QListWidget;preview_=new QLabel("Choose an image");preview_->setAlignment(Qt::AlignCenter);preview_->setMinimumSize(300,240);body->addWidget(gallery_,1);body->addWidget(preview_,2);l->addLayout(body,1);connect(ref,&QPushButton::clicked,this,[this]{refreshGallery();});connect(folder,&QPushButton::clicked,this,[this]{QDesktopServices::openUrl(QUrl::fromLocalFile(library_));});connect(gallery_,&QListWidget::currentRowChanged,this,[this](int){auto*i=gallery_->currentItem();if(!i)return;QPixmap pix(i->data(Qt::UserRole).toString());if(!pix.isNull())preview_->setPixmap(pix.scaled(preview_->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));else preview_->setText("Preview unavailable for this media type. Open it from Library.");});refreshGallery();return p;}
 void refreshGallery(){if(!gallery_)return;gallery_->clear();QStringList exts={"png","jpg","jpeg","gif","bmp","webp","mp4","mov","m4v","avi","mp3","wav"};QDirIterator it(library_,QDir::Files|QDir::NoDotAndDotDot,QDirIterator::Subdirectories);while(it.hasNext()){QFileInfo f(it.next());if(exts.contains(f.suffix().toLower())){auto*i=new QListWidgetItem(QDir(library_).relativeFilePath(f.absoluteFilePath()),gallery_);i->setData(Qt::UserRole,f.absoluteFilePath());}}}
 QWidget*settingsPage(){auto*p=page();auto*l=qobject_cast<QVBoxLayout*>(p->layout());auto*h=new QLabel("Settings");h->setObjectName("hero");l->addWidget(h);auto*d=new QLabel("Local folders and simulator preferences.");d->setObjectName("muted");l->addWidget(d);auto*a=new QPushButton("Open Spectra Imports");auto*b=new QPushButton("Open Spectra Library");auto*c=new QPushButton("Toggle compact mode");l->addWidget(a);l->addWidget(b);l->addWidget(c);connect(a,&QPushButton::clicked,this,[this]{QDesktopServices::openUrl(QUrl::fromLocalFile(imports_));});connect(b,&QPushButton::clicked,this,[this]{QDesktopServices::openUrl(QUrl::fromLocalFile(library_));});connect(c,&QPushButton::clicked,this,[this]{bool val=!settings_->value("compactMode",false).toBool();settings_->setValue("compactMode",val);QMessageBox::information(this,"Saved",val?"Compact mode preference saved.":"Standard spacing preference saved.");});l->addStretch();return p;}
};
int main(int argc,char**argv){const QByteArray pluginPath=QFileInfo(QString::fromLocal8Bit(argv[0])).absolutePath().toLocal8Bit()+"/platforms";qputenv("QT_QPA_PLATFORM_PLUGIN_PATH",pluginPath);QApplication a(argc,argv);Simulator w;w.show();return a.exec();}
