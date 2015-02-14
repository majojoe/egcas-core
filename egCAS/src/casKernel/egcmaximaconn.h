#ifndef EGCMAXIMACONN_H
#define EGCMAXIMACONN_H

#include <QString>
#include <QProcess>
#include <QRegularExpression>
#include "egckernelconn.h"


class EgcMaximaConn : public EgcKernelConn
{
        Q_OBJECT
public:
        /**
         * @brief EgcMaximaConn creates a connection to maxima CAS kernel. This is done by starting the maxima kernel.
         * It waits for commands then and will bring back the results.
         * @param executeMaximaCmd the path to the maxima executable including arguments.
         */
        EgcMaximaConn(QString executeMaximaCmd, QObject *parent = 0);
        ///destructor
        ~EgcMaximaConn();
        /**
         * @brief sendCommand send a command to maxima
         * @param cmd command to be sent to the kernel
         */
        void sendCommand(QString cmd);
        /**
         * @brief quit quit maxima subprocess
         */
        void quit(void);

protected slots:
        virtual void stdOutput(void);
protected:
        static QString s_startupConfig;         ///< startup configuration for CAS kernel
        QRegularExpression m_errUnwantedRegex;  ///< regex for filtering kernel unwanted information from error message
};

#endif // EGCMAXIMACONN_H
