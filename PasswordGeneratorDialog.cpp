#include "PasswordGeneratorDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <time.h>
#include <QWidget>
#include <QPixmap>

PasswordGeneratorDialog::PasswordGeneratorDialog(QWidget *parent) : QDialog(parent)
{
 resize(323, 405);
 setWindowTitle( tr("Password generator") );
 QVBoxLayout *layout = new QVBoxLayout(this);
 stackedWidget = new QStackedWidget(this);
 settingsWidget = new QWidget(stackedWidget);
 uiSettings.setupUi(settingsWidget);
 stackedWidget->addWidget(settingsWidget);
 generatorWidget = new QWidget(this);
 QVBoxLayout *generatorWidgetLayout = new QVBoxLayout(generatorWidget);
 QLabel *label = new QLabel( this );
 label->setText( tr("To generate random characters please move your mouse cursor randomly on checkered pattern below") );
 label->setWordWrap( true );
 generatorWidgetLayout->addWidget(label);
 progressBar = new QProgressBar(this);
 QWidget *graphicsWidget = new QWidget(this);
 graphicsWidget->setMinimumSize(0, IMAGE_HEIGHT+48);
 QHBoxLayout *graphicsWidgetLayout = new QHBoxLayout(graphicsWidget);
 graphicsWidgetLayout->addStretch();
 generator = new GraphicGenerator(progressBar, this );
 generator->setMaximumSize(IMAGE_WIDTH, IMAGE_HEIGHT);
 generator->setMinimumSize(IMAGE_WIDTH, IMAGE_HEIGHT);
 generator->setMouseTracking( true );
 graphicsWidgetLayout->addWidget(generator);
 graphicsWidgetLayout->addStretch();
 generatorWidgetLayout->addWidget(graphicsWidget);
 generatorWidgetLayout->addSpacing(10);
 QLabel *label2 = new QLabel( tr("Generation progress:"), this);
 generatorWidgetLayout->addWidget(label2);
 generatorWidgetLayout->addWidget(progressBar);
 stackedWidget->addWidget(generatorWidget);
 resultWidget = new QWidget(stackedWidget);
 uiResult.setupUi(resultWidget);
 stackedWidget->addWidget(resultWidget);
 layout->addWidget(stackedWidget);
 buttonsWidget = new QWidget(this);
 QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonsWidget);
 buttonsLayout->addStretch();
 backButton = new QPushButton(tr("Back"), this);
 buttonsLayout->addWidget(backButton);
 backButton->setEnabled( false );
 nextButton = new QPushButton(tr("Next"), this);
 buttonsLayout->addWidget(nextButton);
 buttonsLayout->addSpacing(50);
 layout->addWidget(buttonsWidget);
 stackedWidget->setCurrentIndex(0);
 connect(generator, SIGNAL(generatingEnd()), this, SLOT(next()));
 connect(backButton, SIGNAL(clicked()), this, SLOT(back()));
 connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));
}
QString PasswordGeneratorDialog::getResult()
{
 if(uiResult.setAsPasswordBox->isChecked())
  return uiResult.resultEdit->text();
 else
 return QString::Null();
}

void PasswordGeneratorDialog::setSetAsPasswordEnabled(bool enabled)
{
 uiResult.setAsPasswordBox->setEnabled(enabled);
}

void PasswordGeneratorDialog::back()
{
 if(stackedWidget->currentWidget() == generatorWidget)
  {
   stackedWidget->setCurrentWidget( settingsWidget );
   backButton->setEnabled( false );
   nextButton->setEnabled( true );
  }
 else if(stackedWidget->currentWidget() == resultWidget)
 {
 stackedWidget->setCurrentWidget( settingsWidget );
 backButton->setEnabled( false );
 nextButton->setEnabled( true );
 nextButton->setText( tr("Next") );
 }
}

void PasswordGeneratorDialog::next()
{
 if(stackedWidget->currentWidget() == settingsWidget)
  {
    int characterSet = 0;
    if(uiSettings.smallCharactersBox->isChecked())
    characterSet |= smallCharacters;
    if(uiSettings.bigCharactersBox->isChecked())
    characterSet |= bigCharacters;
    if(uiSettings.digitsBox->isChecked())
    characterSet |= digits;
    if(uiSettings.specialCharactersBox->isChecked())
    characterSet |= specialCharacters;
    if(characterSet == 0)
     {
      QMessageBox box(this);
      box.setWindowTitle( tr("PassKeeper") );
      box.setText( tr("You have to select character set!") );
      box.setIcon( QMessageBox::Warning );
      box.exec();
      return;
     }
    generator->initGenerator(uiSettings.lengthBox->value(), characterSet);
    stackedWidget->setCurrentWidget( generatorWidget );
    backButton->setEnabled( true );
    nextButton->setEnabled( false );
	}
 else if(stackedWidget->currentWidget() == generatorWidget)
  {
   stackedWidget->setCurrentWidget( resultWidget );
   backButton->setEnabled( true );
   nextButton->setEnabled( true );
   nextButton->setText( tr("Finish") );
   uiResult.resultEdit->setText( generator->getResult() );
  }
  else if(stackedWidget->currentWidget() == resultWidget)
   {
    accept();
   }
}

GraphicGenerator::GraphicGenerator(QProgressBar *progressBar, QWidget *parent) : QLabel(parent)
{
 this->progressBar = progressBar;
 setPixmap( QPixmap(":/icons/checkmap") );
}

void GraphicGenerator::initGenerator(int size, int characterSet)
{
 this->size = size;
 srand( time(0) );
 for(int i = 0; i < IMAGE_WIDTH; i++)
 for(int j = 0; j < IMAGE_HEIGHT; j++)
 table[i][j] = rand()%256;
 count = 0;
 result.clear();
 characterMapCount = 0;
 if(characterSet & digits)
  {
   for(int i = 0; i < 10; i++)
   characterMap[characterMapCount++] = i + 48;
  }
   if(characterSet & smallCharacters)
   for(int i = 0; i < 26; i++)
    characterMap[characterMapCount++] = i + 97;
   if(characterSet & bigCharacters)
		for(int i = 0; i < 26; i++)
         characterMap[characterMapCount++] = i + 65;
	if(characterSet & specialCharacters)
	{
		for(int i = 0; i < 15; i++)
         characterMap[characterMapCount++] = i + 33;
		for(int i = 0; i < 7; i++)
         characterMap[characterMapCount++] = i + 58;
		for(int i = 0; i < 6; i++)
         characterMap[characterMapCount++] = i + 91;
		for(int i = 0; i < 3; i++)
         characterMap[characterMapCount++] = i + 123;
	}
	setMouseTracking(true);
	progressBar->setValue(0);
	progressBar->setMaximum(size*20);
}

QString GraphicGenerator::getResult()
{
 return result;
}

void GraphicGenerator::mouseMoveEvent( QMouseEvent *event)
{
 value = (table[ event->x() ][ event->y() ] + value) % 256;
 count++;
 progressBar->setValue( progressBar->value()+1);
 if(count == 20)
  {
   result.append( characterMap[ value % characterMapCount ] );
   if(result.count() == size)
    {
     setMouseTracking(false);
     emit generatingEnd();
    }
    count = 0;
  }
}
