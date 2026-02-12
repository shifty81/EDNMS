#include "test_framework.h"
#include "Engine/IO/chunk_format.h"

TEST(ChunkFormat, MagicConstant) {
    EXPECT_EQ(ednms::CHUNK_MAGIC, 0x43484B31u);
    return true;
}

TEST(ChunkFormat, HeaderDefaults) {
    ednms::ChunkHeader header;
    EXPECT_EQ(header.magic, 0u);
    EXPECT_EQ(header.version, 0u);
    EXPECT_EQ(header.chunkID, 0u);
    EXPECT_EQ(header.entityCount, 0u);
    EXPECT_EQ(header.componentCount, 0u);
    return true;
}

TEST(ChunkFormat, HeaderPopulate) {
    ednms::ChunkHeader header;
    header.magic = ednms::CHUNK_MAGIC;
    header.version = 1;
    header.chunkID = 42;
    header.entityCount = 10;
    header.componentCount = 5;

    EXPECT_EQ(header.magic, ednms::CHUNK_MAGIC);
    EXPECT_EQ(header.version, 1u);
    EXPECT_EQ(header.chunkID, 42u);
    EXPECT_EQ(header.entityCount, 10u);
    EXPECT_EQ(header.componentCount, 5u);
    return true;
}
