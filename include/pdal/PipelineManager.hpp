/******************************************************************************
* Copyright (c) 2011, Michael P. Gerlek (mpg@flaxen.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#ifndef INCLUDED_PIPELINEMANAGER_HPP
#define INCLUDED_PIPELINEMANAGER_HPP

#include <pdal/pdal.hpp>
#include <pdal/StageFactory.hpp>

#include <boost/shared_ptr.hpp>

#include <vector>
#include <map>
#include <string>


namespace pdal
{

class Options;


class PDAL_DLL PipelineManager
{
private:
    class StageParserContext;

public:
    PipelineManager();
    ~PipelineManager();

    // Use these to manually add stages into the pipeline manager.
    Reader* addReader(const std::string& type, const Options&);
    Filter* addFilter(const std::string& type, const Stage& prevStage, const Options&);
    MultiFilter* addMultiFilter(const std::string& type, const std::vector<const Stage*>& prevStages, const Options&);
    Writer* addWriter(const std::string& type, const Stage& prevStage, const Options&);
    
    // Use this to fill in a pipeline manager with an XML file that
    // contains a <Writer> as the last pipeline stage.
    Writer& readWriterPipeline(const std::string&);

    // Use this to fill in a pipeline manager with an XML file that 
    // don't contain a <Writer>.  (Even though this is called "parse 
    // READER pipeline", it actually returns a Stage; it can be used 
    // where the last pipeline stage is a Reader or Filter.)
    const Stage& readReaderPipeline(const std::string&);

    void writeWriterPipeline(const std::string& filename) const;

private:
    boost::property_tree::ptree parsePipelineElement(const std::string& filename);
    Writer* parseWriterRoot(const boost::property_tree::ptree&);
    Stage* parseStageRoot(const boost::property_tree::ptree&);

    Stage* parseStageElement(const std::string& name, const boost::property_tree::ptree& subtree);
    Reader* parseReaderElement(const boost::property_tree::ptree& tree);
    Filter* parseFilterElement(const boost::property_tree::ptree& tree);
    MultiFilter* parseMultiFilterElement(const boost::property_tree::ptree& tree);
    Writer* parseWriterElement(const boost::property_tree::ptree& tree);

    Option<std::string> parseOptionElement(const boost::property_tree::ptree& tree);
    std::string parseTypeElement(const boost::property_tree::ptree& tree);

    StageFactory m_factory;

    typedef std::vector<Reader*> ReaderList;
    typedef std::vector<Filter*> FilterList;
    typedef std::vector<MultiFilter*> MultiFilterList;
    typedef std::vector<Writer*> WriterList;
    ReaderList m_readers;
    FilterList m_filters;
    MultiFilterList m_multifilters;
    WriterList m_writers;

    PipelineManager& operator=(const PipelineManager&); // not implemented
    PipelineManager(const PipelineManager&); // not implemented
};


} // namespace pdal

#endif
