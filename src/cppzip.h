#ifndef CPPZIP_H
#define CPPZIP_H

#include <list>
#include <memory>
#include <string>
#include <vector>

#include <zip.h>

/**
 *  ZIP modes (Can be ORed together):

    ZIP_CHECKCONS - Perform additional stricter consistency checks on the archive, and error if they fail.
    ZIP_CREATE - Create the archive if it does not exist.
    ZIP_EXCL - Error if archive already exists.
    ZIP_TRUNCATE - If archive exists, ignore its current contents. In other words, handle it the same way as an empty archive.
    ZIP_RDONLY - Open archive in read-only mode.
 */

class ZIP_Entry
{
    std::string m_name;
    zip_file_t *m_zf;

    ZIP_Entry(std::string name, zip_file_t *zf);
    friend class ZIP;
    friend class OZIP;

public:
    ~ZIP_Entry();

    // is directory or a file
    bool is_directory() const;
    // get entry name
    std::string get_name() const;
    // read entry content
    std::vector<char> read(size_t size) const;
};

class ZIP
{
protected:
    std::string m_name;
    zip_t *m_zip;
    size_t find_idx_by_name(const std::string &name) const;

public:
    ZIP(std::string name, int mode);
    virtual ~ZIP();

    // add file with content
    int add_file(const std::string &name, const void *content, size_t size, bool overwrite = true);
    // add directory
    int add_dir(std::string name);
    // replace existing file
    int replace_file(const std::string &name, const void *content, size_t size);

    // NOTE: after deletion the changes are applied only after close()
    // An attempt to read entry once it was deleted results in an error
    // delete entry by index
    void delete_entry(size_t index);
    // delete entry by name
    void delete_entry(const std::string &name);

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
    // get list of entries
    std::list<std::unique_ptr<ZIP_Entry>> get_entries() const;

    // close file and sync
    bool close() noexcept;
};

class OZIP : private ZIP
{
public:
    OZIP(std::string name, int mode = ZIP_CREATE | ZIP_TRUNCATE);
    ~OZIP() = default;

    using ZIP::add_dir;
    using ZIP::add_file;
    using ZIP::close;

    // add text file
    int add_text_file(const std::string &name, const std::string &content, bool overwrite = true);
};

class IZIP : private ZIP
{
public:
    IZIP(std::string name);
    ~IZIP() = default;

    using ZIP::close;
    using ZIP::get_entries;
    using ZIP::get_entry;
    using ZIP::get_entry_stat;
    using ZIP::get_file_size;
    using ZIP::get_num_entries;
};

#endif