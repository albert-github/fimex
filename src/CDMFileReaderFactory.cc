/*
 * Fimex, CDMFileReaderFactory.cc
 *
 * (C) Copyright 2010-2018, met.no
 *
 * Project Info:  https://wiki.met.no/fimex/start
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 *  Created on: May 5, 2010
 *      Author: Heiko Klein
 */

#include "fimex/CDMFileReaderFactory.h"

#include "fimex/CDMException.h"
#include "fimex/CDMWriter.h"
#include "fimex/CDMconstants.h"
#include "fimex/FileUtils.h"
#include "fimex/IoFactory.h"
#include "fimex/StringUtils.h"
#include "fimex/Type2String.h"
#include "fimex/XMLDoc.h"
#include "fimex/XMLInputDoc.h"
#include "fimex/XMLInputFile.h"
#include "fimex/XMLInputString.h"
#include "fimex/min_max.h"

#include <algorithm>
#include <fstream>
#include <memory>
#include <regex>

namespace MetNoFimex {

#if 0 // FIXME detect xml types, look at this header and xmlns:??="..."
static bool detectXML(const char* magic) {
    return std::regex_match(magic, std::regex("\\s*<\\?xml\\s.*"));
}
#endif

// ========================================================================

namespace {

IoFactory_pm& ioFactories()
{
    return IoFactory::factories();
}

IoFactory_p findFactoryFromFileType(const std::string& fileTypeName)
{
    if (fileTypeName.empty())
        return IoFactory_p();

    IoFactory_p factory;
    int bestType = 0;
    for (const auto& id_f : ioFactories()) {
        if (maximize(bestType, id_f.second->matchFileTypeName(fileTypeName)))
            factory = id_f.second;
    }
    return factory;
}

IoFactory_p findFactoryFromMagic(const std::string& fileName)
{
    size_t magicSize = 0;
    for (const auto& id_f : ioFactories())
        maximize(magicSize, id_f.second->matchMagicSize());

    std::ifstream fs(fileName.c_str());
    if (!fs.is_open())
        throw CDMException("cannot open file '" + fileName + "'");
    std::unique_ptr<char[]> magic(new char[magicSize]);
    fs.read(magic.get(), magicSize);
    const size_t actualMagicSize = fs.gcount();
    fs.close();

    int bestType = 0;
    IoFactory_p factory;
    for (const auto& id_f : ioFactories()) {
        if (maximize(bestType, id_f.second->matchMagic(magic.get(), actualMagicSize)))
            factory = id_f.second;
    }
    return factory;
}

IoFactory_p findFactoryFromFileName(const std::string& fileName)
{
    int bestType = 0;
    IoFactory_p factory;
    for (const auto& id_f : ioFactories()) {
        if (maximize(bestType, id_f.second->matchFileName(fileName)))
            factory = id_f.second;
    }
    return factory;
}

IoFactory_p findFactory(const std::string& fileTypeName, const std::string& fileName, bool write)
{
    if (IoFactory_p f = findFactoryFromFileType(fileTypeName))
        return f;

    if (!write) {
        if (IoFactory_p f = findFactoryFromMagic(fileName))
            return f;
    }

    return findFactoryFromFileName(fileName);
}

XMLInputDoc createXMLInput(const XMLInput& xi)
{
    return XMLInputDoc(xi.id(), xi.getXMLDoc());
}

XMLInputDoc createXMLInput(const std::string& configXML)
{
    if (configXML.empty()) {
        return XMLInputDoc("", XMLDoc_p());
    } else if (starts_with(configXML, "<?xml ")) {
        return createXMLInput(XMLInputString(configXML));
    } else {
        return createXMLInput(XMLInputFile(configXML));
    }
}

} // namespace

// static
CDMReaderWriter_p CDMFileReaderFactory::createReaderWriter(const std::string& fileTypeName, const std::string& fileName, const XMLInput& config,
                                                           const std::vector<std::string>& args)
{
    if (IoFactory_p factory = findFactory(fileTypeName, fileName, false))
        return factory->createReaderWriter(fileTypeName, fileName, config, args);
    throw CDMException("cannot create reader-writer for type '" + fileTypeName + "' and file '" + fileName + "'");
}

// static
CDMReaderWriter_p CDMFileReaderFactory::createReaderWriter(const std::string& fileTypeName, const std::string& fileName, const std::string& configFile,
                                                           const std::vector<std::string>& args)
{
    XMLInputDoc configXML = createXMLInput(configFile);
    return createReaderWriter(fileTypeName, fileName, configXML, args);
}

// static
CDMReader_p CDMFileReaderFactory::create(const std::string& fileTypeName, const std::string& fileName, const XMLInput& configXML,
                                         const std::vector<std::string>& args)
{
    if (IoFactory_p factory = findFactory(fileTypeName, fileName, false))
        return factory->createReader(fileTypeName, fileName, configXML, args);
    throw CDMException("cannot create reader for type '" + fileTypeName + "' and file '" + fileName + "'");
}

// static
CDMReader_p CDMFileReaderFactory::create(const std::string& fileTypeName, const std::string & fileName, const std::string & configFile, const std::vector<std::string> & args)
{
    XMLInputDoc configXML = createXMLInput(configFile);
    return create(fileTypeName, fileName, configXML, args);
}

// static
void CDMFileReaderFactory::createWriter(CDMReader_p input, const std::string& fileTypeName, const std::string& fileName, const std::string& configFile)
{
    if (IoFactory_p factory = findFactory(fileTypeName, fileName, true))
        return factory->createWriter(input, fileTypeName, fileName, configFile);
    throw CDMException("cannot create writer for type '" + fileTypeName + "' and file '" + fileName + "'");
}

} // namespace MetNoFimex
