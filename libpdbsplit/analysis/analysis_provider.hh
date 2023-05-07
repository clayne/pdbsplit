#pragma once

struct s_object_chunks_pair
{
	string_t library_name;
	string_t object_name;
	vec_t<const s_analysis_chunk*> chunks;
};

class c_analysis_provider
{
protected:
	vec_t<s_object_chunks_pair> m_chunks_by_object;
public:
	GETTER(chunks_by_object);

	explicit c_analysis_provider();
	virtual ~c_analysis_provider();
};
