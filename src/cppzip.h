#ifndef CPPZIP_H
#define CPPZIP_H

#include <memory>
#include <string>
#include <vector>

#include <zip.h>

class ZIP_Entry
{
    std::string m_name;
    zip_file_t *m_zf;

    ZIP_Entry(std::string name, zip_file_t *zf);
    friend class ZIP;

public:
    ~ZIP_Entry();

    // no copies
    ZIP_Entry(const ZIP_Entry &) = delete;
    ZIP_Entry &operator=(const ZIP_Entry &) = delete;

    // is directory or a file
    bool is_directory() const;
    // get entry name
    std::string get_name() const;
    // read entry content
    std::vector<char> read(size_t size) const;
};

class ZIP
{
    std::string m_name;
    zip_t *m_zip;

public:
    ZIP(std::string name, int mode);
    virtual ~ZIP();

    // no copies
    ZIP(const ZIP &) = delete;
    ZIP &operator=(const ZIP &) = delete;

    // add file with content
    int add_file(const std::string &name, const void *content, size_t size, bool overwrite = true);
    // add directory
    int add_dir(std::string name);
    // get name
    std::string get_name() const;
    // get archive entries
    size_t get_num_entries() const;
    // get file name by index
    std::string get_entry_name(size_t index) const;
    // get file info by index
    zip_stat_t get_entry_stat(size_t index) const;
    // get file info by name
    zip_stat_t get_entry_stat(const std::string &name) const;
    // get file size by index
    size_t get_file_size(size_t index) const;
    // get file size by name
    size_t get_file_size(const std::string &name) const;
    // get file instance from zip archive by index
    std::unique_ptr<ZIP_Entry> get_entry(size_t index) const;
    // get file instance by name
    std::unique_ptr<ZIP_Entry> get_entry(const std::string &name) const;

    // close file and sync
    bool close() noexcept;
};

#endif