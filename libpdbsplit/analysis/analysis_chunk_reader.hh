#pragma once

class c_analysis_provider
{
public:

	explicit c_analysis_provider();
	virtual ~c_analysis_provider();
};

class c_analysis_chunk_reader
{
public:

	explicit c_analysis_chunk_reader(const c_analysis_provider& provider);
	~c_analysis_chunk_reader();
};
