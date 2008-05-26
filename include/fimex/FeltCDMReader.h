#ifndef FELTCDMREADER_H_
#define FELTCDMREADER_H_

#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include "Felt_File.h"
#include "CDMReader.h"
#include "CDMDimension.h"
#include "ReplaceStringObject.h"

namespace MetNoFimex
{
class XMLDoc; // declaration without import

class FeltCDMReader : public CDMReader
{
public:
	FeltCDMReader(std::string filename, std::string configFilename) throw(CDMException);
	virtual ~FeltCDMReader();
	
	virtual const boost::shared_ptr<Data> getDataSlice(const std::string& varName, size_t unLimDimPos = 0) throw(CDMException);
	virtual const CDM& getCDM() const {return cdm;}
	
private:
	const std::string filename;
	const std::string configFilename;
	MetNoFelt::Felt_File feltFile;
	CDMDimension xDim;
	CDMDimension yDim;
	std::map<std::string, std::string> varNameFeltIdMap;
	std::vector<std::time_t> timeVec;
	std::map<std::string, std::vector<short> > levelVecMap;
	/**
	 * config attributes may contain template parameters marked with %PARAM%
	 * which should be replaced by dynamic values from the felt-file and stored
	 * temporary in this map
	 * 
	 * Currently implemented parameters are: %MIN_DATETIME%, %MAX_DATETIME%: earliest and latest time in felt-file as ISO string
	 */
	std::map<std::string, boost::shared_ptr<ReplaceStringObject> > templateReplacementAttributes;
	void init() throw(MetNoFelt::Felt_File_Error, CDMException);
	// the following methods are parts of the init function and should not
	// be called from elsewhere
	std::vector<std::string> initGetKnownFeltIdsFromXML(const XMLDoc& doc);
	void initAddGlobalAttributesFromXML(const XMLDoc& doc);
	CDMDimension initAddTimeDimensionFromXML(const XMLDoc& doc);
	std::map<short, CDMDimension> initAddLevelDimensionsFromXML(const XMLDoc& doc);
	void initAddProjectionFromXML(const XMLDoc& doc, std::string& projName, std::string& coordinates);
	void initAddVariablesFromXML(const XMLDoc& doc, const std::string& projName, const std::string& coordinates, const CDMDimension& timeDim, const std::map<short, CDMDimension>& levelDims);


};

}

#endif /*FELTCDMREADER_H_*/
