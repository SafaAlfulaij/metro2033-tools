#ifndef __M2033_MODELSERIALIZER_H__
#define __M2033_MODELSERIALIZER_H__

#include "model.h"
#include "prerequisites.h"

namespace m2033
{
	class model_serializer
	{
	public:
		inline model_serializer() {}
		inline ~model_serializer() {}

		enum ChunkIds
		{
			UNUSED_CHUNK_ID = 0x01,
			TEXTURE_NAME_CHUNK_ID = 0x02,
			STATIC_VERTEX_CHUNK_ID = 0x03,
			DYNAMIC_VERTEX_CHUNK_ID = 0x05,
			MODEL_CHUNK_ID = 0x09,
			BONES_CHUNK_ID = 0x0D,
			MESH_NAMES_CHUNK_ID = 0x10,
			SKELETON_NAME_CHUNK_ID = 0x14
		};

		bool read_model( const std::string &file, model &m );
		bool read_mesh( const std::string &file, model::meshes &meshes );
		void read_mesh( int type, reader &r, model::meshes &meshes );
		bool read_skeleton( const std::string &file, skeleton &s );

	private:
		typedef std::list<std::string> string_list;

		void split_string( const std::string& string, char splitter, string_list& result );
	};
}

#endif // __M2033_MODELSERIALIZER_H__