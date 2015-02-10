#ifndef EGCMAXIMACONN_H
#define EGCMAXIMACONN_H

#include <QString>
#include <QProcess>
#include <QRegularExpression>

class EgcMaximaConn : public QObject
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

signals:
        /**
         * @brief resultReceived
         * @param result
         */
        void resultReceived(QString result);
        void errorReceived(QString errorMsg);
        void kernelStarted(void);
        void kernelTerminated(void);
        void kernelErrorOccurred(QProcess::ProcessError error);
protected slots:
        void stdOutput(void);
        void errorOutput(void);
        void kernelStart(void);
        void kernelTerm(void);
        void kernelError(QProcess::ProcessError error);

protected:
        QString m_result;                      ///< stores the result until the result is complete
        QString m_error;                       ///< stores the error message until the message is complete
        QRegularExpression m_regex;            ///< regex for filtering maxima output
        QProcess *m_casKernelProcess;          ///< pointer to the maxima QProcess object
};

#endif // EGCMAXIMACONN_H
