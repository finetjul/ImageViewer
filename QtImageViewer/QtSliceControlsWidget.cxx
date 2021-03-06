/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/

#include "QtSliceControlsWidget.h"
#include <QDebug>


QtSliceControlsWidget::QtSliceControlsWidget(QWidget* parent)
  : QWidget(parent)
{
  this->UI = new Ui::Controls;
  UI->setupUi(this);
}

QtSliceControlsWidget::~QtSliceControlsWidget()
{
}

void QtSliceControlsWidget::setDisplayPosition(double x,double y ,double z,double value)
{
  UI->PositionX->setValue(x);
  UI->PositionY->setValue(y);
  UI->PositionZ->setValue(z);
  UI->PixelValue->setValue(value);
}

void QtSliceControlsWidget::setDisplayIMin(double value)
{
  UI->IntensityMinDisplay->setValue(value);
}

void QtSliceControlsWidget::setDisplayIMax(double value)
{
  UI->IntensityMaxDisplay->setValue(value);
}


void QtSliceControlsWidget::setInputImage()
{
  this->SliceView->setSingleStep(0.02*this->SliceView->intensityRange());
  this->decimalsNumber = significantDecimals(this->SliceView->singleStep(),-1);
  QObject::connect(this->SliceView, SIGNAL(positionChanged(double,double,double,double)),
                   this, SLOT(setDisplayPosition(double,double,double,double)));
  QObject::connect(UI->IntensityMax, SIGNAL(sliderMoved(int)), this,
                   SLOT(setMaxIntensity(int)));
  QObject::connect(UI->IntensityMin, SIGNAL(sliderMoved(int)), this,
                   SLOT(setMinIntensity(int)));
  QObject::connect(UI->ZoomIn, SIGNAL(clicked()), this->SliceView, SLOT(zoomIn()));
  QObject::connect(UI->ZoomOut, SIGNAL(clicked()), this->SliceView, SLOT(zoomOut()));
  QObject::connect(this->SliceView, SIGNAL(maxIntensityChanged(double)), this,
                   SLOT(setDisplayIMax(double)));
  QObject::connect(this->SliceView, SIGNAL(minIntensityChanged(double)), this,
                   SLOT(setDisplayIMin(double)));
  QObject::connect(this->SliceView, SIGNAL(maxIntensityChanged(double)), this,
                   SLOT(setValueIntensityMax(double)));
  QObject::connect(this->SliceView, SIGNAL(minIntensityChanged(double)), this,
                   SLOT(setValueIntensityMin(double)));

  QObject::connect(UI->IntensityMinDisplay,SIGNAL(valueChanged(double)), this->SliceView,
                   SLOT(setMinIntensity(double)));
  QObject::connect(UI->IntensityMinDisplay,SIGNAL(valueChanged(double)), this,
                   SLOT(setValueIntensityMin(double)));
  QObject::connect(this->SliceView, SIGNAL(minIntensityChanged(double)), UI->IntensityMinDisplay,
                   SLOT(setValue(double)));

  QObject::connect(UI->IntensityMaxDisplay,SIGNAL(valueChanged(double)), this,
                   SLOT(setValueIntensityMax(double)));
  QObject::connect(UI->IntensityMaxDisplay,SIGNAL(valueChanged(double)), this->SliceView,
                   SLOT(setMaxIntensity(double)));
  QObject::connect(this->SliceView, SIGNAL(maxIntensityChanged(double)), UI->IntensityMaxDisplay,
                   SLOT(setValue(double)));

  QObject::connect(this->SliceView, SIGNAL(detailsChanged(QString)), this,
                    SLOT(setText(QString)));

  UI->IntensityMin->setMinimum(0);
  UI->IntensityMin->setMaximum( static_cast<int>
            ( this->SliceView->intensityRange()/this->SliceView->singleStep()));
  UI->IntensityMin->setValue(0);
  UI->IntensityMax->setMinimum(0);
  UI->IntensityMax->setMaximum( static_cast<int>
            ( this->SliceView->intensityRange()/this->SliceView->singleStep()));
  UI->IntensityMax->setValue( static_cast<int>
            ( this->SliceView->intensityRange()/this->SliceView->singleStep()));

  UI->IntensityMinDisplay->setMinimum( this->SliceView->minIntensity() );
  UI->IntensityMinDisplay->setMaximum( this->SliceView->maxIntensity() );

  UI->IntensityMaxDisplay->setMinimum( this->SliceView->minIntensity() );
  UI->IntensityMaxDisplay->setMaximum( this->SliceView->maxIntensity() );

  UI->IntensityMinDisplay->setValue( this->SliceView->minIntensity() );
  UI->IntensityMinDisplay->setDecimals(this->decimalsNumber);
  UI->IntensityMaxDisplay->setValue( this->SliceView->maxIntensity() );
  UI->IntensityMaxDisplay->setDecimals(this->decimalsNumber);
  UI->IntensityMinDisplay->setSingleStep( this->SliceView->singleStep() );
  UI->IntensityMaxDisplay->setSingleStep( this->SliceView->singleStep() );

  UI->PositionX->setMaximum(this->SliceView->imageSize(0));
  UI->PositionX->setDecimals(this->decimalsNumber);
  UI->PositionY->setMaximum(this->SliceView->imageSize(1));
  UI->PositionY->setDecimals(this->decimalsNumber);
  UI->PositionZ->setMaximum(this->SliceView->imageSize(2));
  UI->PositionZ->setDecimals(this->decimalsNumber);
  UI->PixelValue->setMinimum(this->SliceView->minIntensity());
  UI->PixelValue->setMaximum(this->SliceView->maxIntensity());
  UI->PixelValue->setDecimals(this->decimalsNumber);
}

void QtSliceControlsWidget::setSliceView(QtGlSliceView* sliceView)
{
  this->SliceView = sliceView;
}

void QtSliceControlsWidget::setText(const QString& text)
{
  this->UI->Details->setText(text);
}

void QtSliceControlsWidget::setTextVisible(bool visible)
{
  this->UI->Details->setVisible(visible);
}

void QtSliceControlsWidget::setValueIntensityMin(double value)
{
  UI->IntensityMin->setValue(value/this->SliceView->singleStep());
}

void QtSliceControlsWidget::setValueIntensityMax(double value)
{
  UI->IntensityMax->setValue(value/this->SliceView->singleStep());
}

void QtSliceControlsWidget::setMinIntensity(int intensity)
{
  double newMinIntensity = intensity* this->SliceView->singleStep();
  this->SliceView->setMinIntensity(newMinIntensity);
}

void QtSliceControlsWidget::setMaxIntensity(int intensity)
{
  double newMaxIntensity = intensity* this->SliceView->singleStep();
  this->SliceView->setMaxIntensity(newMaxIntensity);
}

/// CTK methods, comes from ctkUtils.cxx class
int QtSliceControlsWidget::significantDecimals(double value, int defaultDecimals)
{
  if (value == 0.
      || qAbs(value) == std::numeric_limits<double>::infinity())
    {
    return 0;
    }
  if (value != value) // is NaN
    {
    return -1;
    }
  QString number = QString::number(value, 'f', 16);
  QString fractional = number.section('.', 1, 1);
  Q_ASSERT(fractional.length() == 16);
  QChar previous;
  int previousRepeat=0;
  bool only0s = true;
  bool isUnit = value > -1. && value < 1.;
  for (int i = 0; i < fractional.length(); ++i)
    {
    QChar digit = fractional.at(i);
    if (digit != '0')
      {
      only0s = false;
      }
    // Has the digit been repeated too many times ?
    if (digit == previous && previousRepeat == 2 &&
        !only0s)
      {
      if (digit == '0' || digit == '9')
        {
        return i - previousRepeat;
        }
      return i;
      }
    // Last digit
    if (i == fractional.length() - 1)
      {
      // If we are here, that means that the right number of significant
      // decimals for the number has not been figured out yet.
      if (previousRepeat > 2 && !(only0s && isUnit) )
        {
        return i - previousRepeat;
        }
      // If defaultDecimals has been provided, just use it.
      if (defaultDecimals >= 0)
        {
        return defaultDecimals;
        }
      return fractional.length();
      }
    // get ready for next
    if (previous != digit)
      {
      previous = digit;
      previousRepeat = 1;
      }
    else
      {
      ++previousRepeat;
      }
    }
  Q_ASSERT(false);
  return fractional.length();
}
