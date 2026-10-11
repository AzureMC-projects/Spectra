#include <QApplication>
#include <QDir>
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
#include <QTextBrowser>
#ifdef SPECTRA_WEBENGINE
#include <QWebEngineView>
#endif

class Simulator final:public QMainWindow{
public:
 Simulator(){setWindowTitle("Spectra Simulator");resize(1360,850);setMinimumSize(980,650);docs_=QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);imports_=QDir(docs_).filePath("Spectra Imports");library_=QDir(docs_).filePath("Spectra Library");QDir().mkpath(imports_);QDir().mkpath(library_);settings_=new QSettings("Spectra","Simulator",this);
 setStyleSheet("QMainWindow,QWidget#page{background:#080d18;color:#f5f7ff} QLabel{color:#f5f7ff} QLabel#hero{font-size:30px;font-weight:800} QLabel#muted{color:#9eabc3} QFrame#top{background:#101a2b;border-bottom:1px solid #2b3c58} QPushButton{color:white;background:#16243a;border:1px solid #344c70;border-radius:9px;padding:10px 15px} QPushButton:hover{background:#24466f;border-color:#82c8ff} QPushButton#tile{text-align:left;min-width:160px;min-height:110px;font-size:17px;font-weight:700;background:#111f34} QPushButton#primary{background:#d9ecff;color:#07162a;font-weight:800} QLineEdit,QListWidget,QTextBrowser{color:#f5f7ff;background:#0d1828;border:1px solid #2b3c58;padding:8px}");
 auto *c=new QWidget;auto *v=new QVBoxLayout(c);v->setContentsMargins(0,0,0,0);auto *top=new QWidget;top->setObjectName("top");auto *th=new QHBoxLayout(top);auto *brand=new QLabel("S P E C T R A");brand->setStyleSheet("font-size:21px;font-weight:900;color:#8dcaff");th->addWidget(brand);th->addStretch();th->addWidget(new QLabel(QDateTime::currentDateTime().toString("ddd d MMM  h:mm AP")));v->addWidget(top);
 auto *nav=new QHBoxLayout;QStringList names={"Home","Library","Browser","Store","Media Gallery","Settings"};for(int i=0;i<names.size();++i){auto *b=new QPushButton(names[i]);nav->addWidget(b);connect(b,&QPushButton::clicked,this,[this,i]{showPage(i);});}auto *nw=new QWidget;nw->setLayout(nav);v->addWidget(nw);
 pages_=new QStackedWidget;pages_->addWidget(home());pages_->addWidget(libraryPage());pages_->addWidget(browserPage());pages_->addWidget(storePage());pages_->addWidget(galleryPage());pages_->addWidget(settingsPage());v->addWidget(pages_,1);setCentralWidget(c);showPage(0);}
private:
 QString docs_,imports_,library_;QSettings *settings_{};QStackedWidget *pages_{};QListWidget *lib_{},*gallery_{};QLabel *preview_{};QLineEdit *address_{};
#ifdef SPECTRA_WEBENGINE
 QWebEngineView *web_{};
#endif
 QWidget* page(){auto*p=new QWidget;p->setObjectName("page");auto*l=new QVBoxLayout(p);l->setContentsMargins(34,26,34,26);l->setSpacing(15);return p;}
 void showPage(int i){pages_->setCurrentIndex(i);if(i==1)refreshLib();if(i==4)refreshGallery();
#ifdef SPECTRA_WEBENGINE
 if(i==2&&web_->url().isEmpty())web_->setUrl(QUrl("https://www.google.com"));
#endif
 }
 void addTile(QGridLayout*g,QString text,int pageIndex){auto*b=new QPushButton(text);b->setObjectName("tile");g->addWidget(b);connect(b,&QPushButton::clicked,this,[this,pageIndex]{showPage(pageIndex);});}
 QWidget*home(){auto*p=page();auto*l=qobject_cast<QVBoxLayout*>(p->layout());auto*ey=new QLabel("WELCOME BACK");ey->setObjectName("muted");l->addWidget(ey);auto*h=new QLabel("Your games. Your media. Your space.");h->setObjectName("hero");l->addWidget(h);auto*s=new QLabel("A polished console-inspired desktop shell.");s->setObjectName("muted");l->addWidget(s);auto*g=new QGridLayout;g->setSpacing(14);addTile(g,"▦   Library\n\nBrowse injected files",1);addTile(g,"◎   Internet Browser\n\nBrowse inside Spectra",2);addTile(g,"▣   Store\n\nOfficial storefront links",3);addTile(g,"▧   Media Gallery\n\nView local photos",4);addTile(g,"⚙   Settings\n\nSimulator preferences",5);auto*imp=new QPushButton("＋   Open imports folder");g->addWidget(imp);connect(imp,&QPushButton::clicked,this,[this]{QDesktopServices::openUrl(QUrl::fromLocalFile(imports_));});l->addLayout(g);l->addStretch();auto*f=new QLabel("SPECTRA SIMULATOR • Windows desktop shell • Not a PS5 emulator");f->setObjectName("muted");l->addWidget(f);return p;}
 QWidget*libraryPage(){auto*p=page();auto*l=qobject_cast<QVBoxLayout*>(p->layout());auto*h=new QLabel("Library");h->setObjectName("hero");l->addWidget(h);auto*d=new QLabel("Files copied by Spectra appear here. Supported files open using their associated Windows app.");d->setObjectName("muted");d->setWordWrap(true);l->addWidget(d);auto*r=new QHBoxLayout;auto*ref=new QPushButton("Refresh");auto*folder=new QPushButton("Open library folder");r->addWidget(ref);r->addWidget(folder);r->addStretch();l->addLayout(r);lib_=new QListWidget;l->addWidget(lib_,1);auto*open=new QPushButton("Open selected file");open->setObjectName("primary");l->addWidget(open);connect(ref,&QPushButton::clicked,this,[this]{refreshLib();});connect(folder,&QPushButton::clicked,this,[this]{QDesktopServices::openUrl(QUrl::fromLocalFile(library_));});connect(open,&QPushButton::clicked,this,[this]{auto*i=lib_->currentItem();if(!i)return;QString path=i->data(Qt::UserRole).toString();QString ext=QFileInfo(path).suffix().toLower();if(!QStringList({"txt","pdf","png","jpg","jpeg","gif","bmp","webp","html","htm","mp4","mp3","wav","m4v","mov"}).contains(ext)){QMessageBox::information(this,"Unsupported file","Spectra will not execute this file type.");return;}QDesktopServices::openUrl(QUrl::fromLocalFile(path));});refreshLib();return p;}
 void refreshLib(){if(!lib_)return;lib_->clear();for(auto f:QDir(library_).entryInfoList(QDir::Files|QDir::NoDotAndDotDot,QDir::Name)){auto*i=new QListWidgetItem(f.fileName()+" ("+QString::number(f.size())+" bytes)",lib_);i->setData(Qt::UserRole,f.absoluteFilePath());}}
 QWidget*browserPage(){auto*p=page();auto*l=qobject_cast<QVBoxLayout*>(p->layout());auto*h=new QLabel("Internet Browser");h->setObjectName("hero");l->addWidget(h);auto*r=new QHBoxLayout;auto*back=new QPushButton("←");auto*forward=new QPushButton("→");auto*reload=new QPushButton("↻");auto*home=new QPushButton("Home");address_=new QLineEdit("https://www.google.com");auto*go=new QPushButton("Go");go->setObjectName("primary");for(auto*b:{back,forward,reload,home})r->addWidget(b);r->addWidget(address_,1);r->addWidget(go);l->addLayout(r);
#ifdef SPECTRA_WEBENGINE
 web_=new QWebEngineView;l->addWidget(web_,1);auto navigate=[this]{QString s=address_->text().trimmed();if(s.isEmpty())return;if(!s.contains("://"))s=(s.contains('.')&&!s.contains(' '))?"https://"+s:"https://www.google.com/search?q="+QString(QUrl::toPercentEncoding(s));web_->setUrl(QUrl(s));};connect(go,&QPushButton::clicked,this,navigate);connect(address_,&QLineEdit::returnPressed,this,navigate);connect(back,&QPushButton::clicked,web_,&QWebEngineView::back);connect(forward,&QPushButton::clicked,web_,&QWebEngineView::forward);connect(reload,&QPushButton::clicked,web_,&QWebEngineView::reload);connect(home,&QPushButton::clicked,this,[this]{address_->setText("https://www.google.com");web_->setUrl(QUrl(address_->text()));});connect(web_,&QWebEngineView::urlChanged,this,[this](const QUrl&u){address_->setText(u.toString());});
#else
 auto*note=new QTextBrowser;note->setHtml("<h2>Browser module unavailable</h2><p>This build needs Qt WebEngine installed to show real websites inside Spectra.</p>");l->addWidget(note,1);auto warn=[this]{QMessageBox::information(this,"Browser unavailable","Rebuild with Qt WebEngine installed.");};connect(go,&QPushButton::clicked,this,warn);connect(address_,&QLineEdit::returnPressed,this,warn);connect(back,&QPushButton::clicked,this,warn);connect(forward,&QPushButton::clicked,this,warn);connect(reload,&QPushButton::clicked,this,warn);connect(home,&QPushButton::clicked,this,warn);
#endif
 return p;}
 QWidget*storePage(){auto*p=page();auto*l=qobject_cast<QVBoxLayout*>(p->layout());auto*h=new QLabel("Store");h->setObjectName("hero");l->addWidget(h);auto*n=new QLabel("Official pages open in the simulator browser.");n->setObjectName("muted");l->addWidget(n);QList<QPair<QString,QString>> links={{"PlayStation Store","https://store.playstation.com/"},{"PlayStation Plus","https://www.playstation.com/ps-plus/"},{"PlayStation Support","https://www.playstation.com/support/"}};for(auto e:links){auto*b=new QPushButton(e.first+" →");l->addWidget(b);connect(b,&QPushButton::clicked,this,[this,e]{
#ifdef SPECTRA_WEBENGINE
 showPage(2);address_->setText(e.second);web_->setUrl(QUrl(e.second));
#else
 showPage(2);address_->setText(e.second);
#endif
 });}l->addStretch();return p;}
 QWidget*galleryPage(){auto*p=page();auto*l=qobject_cast<QVBoxLayout*>(p->layout());auto*h=new QLabel("Media Gallery");h->setObjectName("hero");l->addWidget(h);auto*row=new QHBoxLayout;auto*ref=new QPushButton("Refresh");auto*folder=new QPushButton("Open library folder");row->addWidget(ref);row->addWidget(folder);row->addStretch();l->addLayout(row);auto*body=new QHBoxLayout;gallery_=new QListWidget;preview_=new QLabel("Choose an image");preview_->setAlignment(Qt::AlignCenter);preview_->setMinimumSize(300,240);body->addWidget(gallery_,1);body->addWidget(preview_,2);l->addLayout(body,1);connect(ref,&QPushButton::clicked,this,[this]{refreshGallery();});connect(folder,&QPushButton::clicked,this,[this]{QDesktopServices::openUrl(QUrl::fromLocalFile(library_));});connect(gallery_,&QListWidget::currentRowChanged,this,[this](int){auto*i=gallery_->currentItem();if(!i)return;QPixmap pix(i->data(Qt::UserRole).toString());if(!pix.isNull())preview_->setPixmap(pix.scaled(preview_->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));else preview_->setText("Preview unavailable for this media type. Open it from Library.");});refreshGallery();return p;}
 void refreshGallery(){if(!gallery_)return;gallery_->clear();QStringList exts={"png","jpg","jpeg","gif","bmp","webp","mp4","mov","m4v","avi","mp3","wav"};for(auto f:QDir(library_).entryInfoList(QDir::Files|QDir::NoDotAndDotDot,QDir::Name))if(exts.contains(f.suffix().toLower())){auto*i=new QListWidgetItem(f.fileName(),gallery_);i->setData(Qt::UserRole,f.absoluteFilePath());}}
 QWidget*settingsPage(){auto*p=page();auto*l=qobject_cast<QVBoxLayout*>(p->layout());auto*h=new QLabel("Settings");h->setObjectName("hero");l->addWidget(h);auto*d=new QLabel("Local folders and simulator preferences.");d->setObjectName("muted");l->addWidget(d);auto*a=new QPushButton("Open Spectra Imports");auto*b=new QPushButton("Open Spectra Library");auto*c=new QPushButton("Toggle compact mode");l->addWidget(a);l->addWidget(b);l->addWidget(c);connect(a,&QPushButton::clicked,this,[this]{QDesktopServices::openUrl(QUrl::fromLocalFile(imports_));});connect(b,&QPushButton::clicked,this,[this]{QDesktopServices::openUrl(QUrl::fromLocalFile(library_));});connect(c,&QPushButton::clicked,this,[this]{bool val=!settings_->value("compactMode",false).toBool();settings_->setValue("compactMode",val);QMessageBox::information(this,"Saved",val?"Compact mode preference saved.":"Standard spacing preference saved.");});l->addStretch();return p;}
};
int main(int argc,char**argv){QApplication a(argc,argv);Simulator w;w.show();return a.exec();}
