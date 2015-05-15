#ifndef EGCKERNELPARSER_H
#define EGCKERNELPARSER_H

class EgcNode;
class EgcBaseNode;

#include <QString>
#include "egcbasenode.h"

/**
 * @brief The EgcKernelParser class encapsulates the parsing of the cas kernel output and creates a node tree thereof.
 */
class EgcKernelParser
{
public:
        EgcKernelParser();
        virtual ~EgcKernelParser() {}

        /**
         * @brief parseKernelOutput parse the cas kernel output and generate a expression tree from it.
         * @param strToParse the output of the cas kernel to generate a tree from
         * @param result the tree generated. A nullptr if an error ocurred.
         * @return true if everything was fine, false otherwise.
         */
        bool parseKernelOutput(const QString&strToParse, EgcBaseNode** result);

        /**
         * @brief getErrorMessage the error message if an error while parsing ocurred.
         * @return the error message if one.
         */
        QString getErrorMessage();
private:
        QString m_errMessage;   /// stores a error message if an error ocurred while parsing
};

#endif // EGCKERNELPARSER_H
