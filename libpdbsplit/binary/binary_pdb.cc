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

// naive as all hell
void fill_names_for_contribution(
	const vec_t<const PDB::CodeView::DBI::Record*>& public_names,
	const PDB::DBI::SectionContribution* section_contribution,
	const PDB::ImageSectionStream* image_section_stream,
	vec_t<const PDB::CodeView::DBI::Record*>& public_names_out)
{
	const PDB::CodeView::DBI::Record* head = nullptr;

	uint32_t rva = image_section_stream->ConvertSectionOffsetToRVA(
		section_contribution->section,
		section_contribution->offset);

	uint32_t end = rva + section_contribution->size;
	
	for (const auto public_name : public_names)
	{
		uint32_t public_name_rva = image_section_stream->ConvertSectionOffsetToRVA(
			public_name->data.S_PUB32.section,
			public_name->data.S_PUB32.offset);

		if (public_name_rva >= rva)
		{
			if (public_name_rva < end)
			{
				public_names_out.push_back(public_name);
			}
			//else
			//{
			//	break;
			//}
		}
	}
}

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
		vec_t<const PDB::CodeView::DBI::Record*> associated_public_names;
		fill_names_for_contribution(public_names, &contribution, &image_section_stream, associated_public_names);
	}
}

c_binary_format_pdb::~c_binary_format_pdb()
{

}
