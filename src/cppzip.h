#ifndef CPPZIP_H
#define CPPZIP_H

#include <string>
#include <vector>
#include <memory>

#include <zip.h>

class ZIP_File
{
    zip_file_t *m_zf;

    ZIP_File(zip_file_t *zf);
    friend class ZIP;

public:
    ~ZIP_File();

    ZIP_File(const ZIP_File &) = delete;
    ZIP_File &operator=(const ZIP_File &) = delete;

    std::vector<char> read(size_t size);
};

class ZIP
{
    zip_t *m_zip;

public:
    ZIP(const std::string &name, int mode);
    virtual ~ZIP();

    ZIP(const ZIP &) = delete;
    ZIP &operator=(const ZIP &) = delete;

    int add_file(const std::string &name, const void *content, size_t size, bool overwrite = true);
    size_t get_num_entries() const;
    std::string get_file_name(size_t index);
    zip_stat_t get_file_stat(size_t index);
    size_t get_file_size(size_t index);
    std::unique_ptr<ZIP_File> get_file(size_t index);
    bool close();
};

#endif