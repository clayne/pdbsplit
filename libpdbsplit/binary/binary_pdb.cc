#include <PDB.h>
#include <PDB_RawFile.h>
#include <PDB_DBIStream.h>

static inline uint32_t binary_search_get_public_symbol_rva(
	const PDB::CodeView::DBI::Record* data,
	uint32_t search_middle,
	const PDB::ImageSectionStream* userdata)
{
	return data[search_middle].data.S_PUB32.offset;
}

void fill_names_for_contribution(
	const vec_t<const PDB::CodeView::DBI::Record*>& public_names,
	const PDB::DBI::SectionContribution* section_contribution,
	const PDB::ImageSectionStream* image_section_stream)
{

}

//static inline void fill_public_symbols_for_range(
//	const std::vector<s_public_symbol>& symbols,
//	std::vector<const s_public_symbol*>& names,
//	uint32_t start,
//	uint32_t end)
//{
//
//
//	const s_public_symbol* data = symbols.data();
//	const s_public_symbol* head = nullptr;
//	
//	binary_search<const s_public_symbol>(
//		data,
//		symbols.size(),
//		binary_search_get_public_symbol_rva,
//		start,
//		head);
//	
//	// 99% of cases: public name at the start of a section contribution
//	if (head)
//	{
//		while (head->rva < end)
//		{
//			names.push_back(head);
//			head++;
//		}
//	}
//	else
//	{
//		for (const s_public_symbol& symbol : symbols)
//		{
//			if (symbol.rva >= start)
//			{
//				if (symbol.rva < end)
//				{
//					names.push_back(&symbol);
//				}
//				else
//				{
//					break;
//				}
//			}
//		}
//	}
//}

c_binary_format_pdb::c_binary_format_pdb(const s_binary_view& data)
{
	assert(PDB::ValidateFile(data.data()) == PDB::ErrorCode::Success);
	PDB::RawFile raw_pdb = PDB::CreateRawFile(data.data());
	PDB::DBIStream dbi_stream = PDB::CreateDBIStream(raw_pdb);
	PDB::ImageSectionStream image_section_stream = dbi_stream.CreateImageSectionStream(raw_pdb);
	PDB::SectionContributionStream section_contribution_stream = dbi_stream.CreateSectionContributionStream(raw_pdb);
	PDB::CoalescedMSFStream symbol_record_stream = dbi_stream.CreateSymbolRecordStream(raw_pdb);
	PDB::PublicSymbolStream public_symbol_stream = dbi_stream.CreatePublicSymbolStream(raw_pdb);
	PDB::ModuleInfoStream module_info_stream = dbi_stream.CreateModuleInfoStream(raw_pdb);

	vec_t<const PDB::CodeView::DBI::Record*> public_names;
	for (const auto& hash_record : public_symbol_stream.GetRecords())
	{
		const PDB::CodeView::DBI::Record* record = public_symbol_stream.GetRecord(
			symbol_record_stream, hash_record);

		// end of data? THIS SHOULD NOT BE HAPPENING
		if (!record)
		{
			break;
		}

		const uint32_t rva = image_section_stream.ConvertSectionOffsetToRVA(
			record->data.S_PUB32.section, record->data.S_PUB32.offset);

		// certain symbols (e.g. control-flow guard symbols) don't have a valid RVA, ignore those
		if (rva != 0)
		{
			public_names.push_back(record);
		}
	}

	for (const auto& contribution : section_contribution_stream.GetContributions())
	{
		//vec_t<>
	}
}

c_binary_format_pdb::~c_binary_format_pdb()
{

}
