/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>

#include "analogclock.h"
#include <QXmlInputSource>
#include <QXmlStreamReader>
#include <QtDebug>
#include "cdata.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //open GPX File
    QFile file("../analogclock/Sortie_v_lo_le_midi.gpx");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Cannot read file :" << file.errorString();
        exit(0);
    }

    QList<CData> list;

    QXmlStreamReader reader(&file);
    qDebug() << "Start Parsing";
    if (reader.readNextStartElement())
    {
        qDebug()<< "first start element" <<reader.name().toString();
        if (reader.name() == "gpx")
        {
            while (!reader.atEnd())
            {
                if (reader.isStartElement() && reader.name()== "trkpt")
                {
                    CData item;
                    QString attribute_value;

                    //Recuperation des 2 attributs
                    foreach(const QXmlStreamAttribute &attr, reader.attributes())
                    {
                        if (attr.name().toString() == QLatin1String("lat"))
                        {
                            attribute_value = attr.value().toString();
                            //qDebug() << " - lat : " << qPrintable(attribute_value);
                            item.lat = attribute_value.toDouble();
                        }
                        if (attr.name().toString() == QLatin1String("lon"))
                        {
                            attribute_value = attr.value().toString();
                            item.lon = attribute_value.toDouble();
                        }
                     }
                    //recuperation des infos à l'intérieur de l'élément
                    while (!(reader.isEndElement() && reader.name()== "trkpt"))
                    {
                        if (reader.isStartElement() && reader.name()== "ele")
                        {
                           item.alt = reader.readElementText().toInt();
                        }
                        if (reader.isStartElement() && reader.name()== "time")
                        {
                           QStringList lst = reader.readElementText().split("T");
                           item.day =  QDate::fromString(lst.at(0),"yyyy-MM-dd");  //2018-09-17T10:19:56Z
                           item.hours = QTime::fromString(lst.at(1).split("Z").at(0),"hh:mm:ss");
                        }
                        if (reader.isStartElement() && reader.name()== "atemp")
                        {
                           item.temp =  reader.readElementText().toInt();
                        }
                        if (reader.isStartElement() && reader.name()== "hr")
                        {
                           item.bpm = reader.readElementText().toInt();
                        }
                        if (reader.isStartElement() && reader.name()== "cad")
                        {
                           item.cad = reader.readElementText().toInt();
                        }
                        reader.readNext();
                    }
                   //add element in the list
                    list.append(item);
                }
                reader.readNext();
            }

            QList<CData>::iterator i;
            qDebug()<<"Number of elements <"<<list.size()<<">";
            for (i = list.begin(); i!=list.end(); ++i)
            {
                CData item = *i;
                qDebug()<< "Point : lat "<< (item.lat)
                        << ", lon "<< (item.lon)
                        << ", hr "<< (item.bpm)
                        << ", alt "<< (item.alt)
                        << ", temp "<< (item.temp)
                        << ", cad "<< (item.cad)
                        << ", day "<< (item.day.toString())
                        << ", hours "<< (item.hours.toString());
             }


            /*while(reader.readNextStartElement())
            {
                qDebug() << "reader : " << reader.name();
                if(reader.name() == "trkpt")
                {
                    qDebug("trkpt found");
                    while(reader.readNextStartElement())
                    {
                        if (reader.name() == "time")
                        {
                                qDebug() << "time : " << reader.readElementText();
                        }
                        else
                        {
                            //qDebug() << "name : " << reader.name();
                            reader.skipCurrentElement();
                        }
                     }//trk
                }
               // else
                    //reader.skipCurrentElement();
            }*/
        }
        else
          qDebug("not gpx element");
    }


    AnalogClock clock;
    clock.show();
    return app.exec();
}
