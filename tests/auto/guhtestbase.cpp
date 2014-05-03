/****************************************************************************
 *                                                                          *
 *  This file is part of guh.                                               *
 *                                                                          *
 *  Guh is free software: you can redistribute it and/or modify             *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, version 2 of the License.                 *
 *                                                                          *
 *  Guh is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *  You should have received a copy of the GNU General Public License       *
 *  along with guh.  If not, see <http://www.gnu.org/licenses/>.            *
 *                                                                          *
 ***************************************************************************/

#include "guhtestbase.h"
#include "mocktcpserver.h"
#include "guhcore.h"
#include "devicemanager.h"

#include <QVariantMap>
#include <QJsonDocument>
#include <QSignalSpy>
#include <QSettings>
#include <QtTest>
#include <QMetaType>
#include "testdefines.h"

Q_IMPORT_PLUGIN(DevicePluginMock)

GuhTestBase::GuhTestBase(QObject *parent) :
    QObject(parent),
    m_commandId(0)
{
    m_mockDevice1Port = 1337;
    m_mockDevice2Port = 7331;
    QCoreApplication::instance()->setOrganizationName("guh-test");

}

void GuhTestBase::initTestCase()
{

    // If testcase asserts cleanup won't do. Lets clear any previous test run settings leftovers
    QSettings settings;
    settings.clear();

    GuhCore::instance();

    // Wait for the DeviceManager to signal that it has loaded plugins and everything
    QSignalSpy spy(GuhCore::instance()->deviceManager(), SIGNAL(loaded()));
    QVERIFY(spy.isValid());
    QVERIFY(spy.wait());

    m_mockTcpServer = MockTcpServer::servers().first();
    m_clientId = QUuid::createUuid();

    // Lets add one instance of the mockdevice
    QVariantMap params;
    params.insert("deviceClassId", "{753f0d32-0468-4d08-82ed-1964aab03298}");
    QVariantMap deviceParams;
    deviceParams.insert("httpport", m_mockDevice1Port);
    params.insert("deviceParams", deviceParams);
    QVariant response = injectAndWait("Devices.AddConfiguredDevice", params);

    QCOMPARE(response.toMap().value("params").toMap().value("success").toBool(), true);

    m_mockDeviceId = DeviceId(response.toMap().value("params").toMap().value("deviceId").toString());
    QVERIFY2(!m_mockDeviceId.isNull(), "Newly created mock device must not be null.");
}

void GuhTestBase::cleanupTestCase()
{
    GuhCore::instance()->destroy();
}

QVariant GuhTestBase::injectAndWait(const QString &method, const QVariantMap &params)
{
    QVariantMap call;
    call.insert("id", m_commandId++);
    call.insert("method", method);
    call.insert("params", params);

    QJsonDocument jsonDoc = QJsonDocument::fromVariant(call);
    QSignalSpy spy(m_mockTcpServer, SIGNAL(outgoingData(QUuid,QByteArray)));

    m_mockTcpServer->injectData(m_clientId, jsonDoc.toJson());

    if (spy.count() == 0) {
        spy.wait();
    }

     // Make sure the response it a valid JSON string
     QJsonParseError error;
     jsonDoc = QJsonDocument::fromJson(spy.takeFirst().last().toByteArray(), &error);

     return jsonDoc.toVariant();
}

int main(int argc, char *argv[])
{
    REGISTER_METATYPES

    QGuiApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    bool success = true;
    qDebug() << "got testcases" << TESTCASES;
    foreach (const QString &testcase, QString(TESTCASES).split(' ')) {
        int id = QMetaType::type(testcase.toLatin1().data());
        if (id != -1) {
           GuhTestBase *testcase = static_cast<GuhTestBase*>(QMetaType::create(id));
           success &= QTest::qExec(testcase, argc, argv);
           delete testcase;
           QCoreApplication::processEvents();
       }
    }
    return success;
}