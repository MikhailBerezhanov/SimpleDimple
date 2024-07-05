#include <stdexcept>

#include "cppzip.h"

/**
 * ZIP_Entry
 */

ZIP_Entry::ZIP_Entry(std::string name, zip_file_t *zf)
    : m_name(std::move(name))
    , m_zf(zf)
{}

ZIP_Entry::~ZIP_Entry()
{
    zip_fclose(m_zf);
}

bool ZIP_Entry::is_directory() const
{
    return m_name.back() == '/';
}

std::string ZIP_Entry::get_name() const
{
    return m_name;
}

std::vector<char> ZIP_Entry::read(size_t size) const
{
    if (is_directory())
    {
        throw std::runtime_error(m_name + " read() not applicable to directories");
    }

    std::vector<char> buffer(size);
    auto bytes_read = zip_fread(m_zf, buffer.data(), size);
    if (bytes_read < 0)
    {
        throw std::runtime_error(m_name + " error reading from zip file");
    }
    return buffer;
}

/**
 * ZIP
 */

ZIP::ZIP(std::string name, int mode)
    : m_name(std::move(name))
{
    int err;
    m_zip = zip_open(m_name.c_str(), mode, &err);
    if (m_zip == nullptr)
    {
        zip_error_t zipError;
        zip_error_init_with_code(&zipError, err);
        std::string zip_err_str = zip_error_strerror(&zipError);
        zip_error_fini(&zipError);
        throw std::runtime_error(m_name + " zip error " + zip_err_str);
    }
}

ZIP::~ZIP()
{
    if (m_zip)
        zip_close(m_zip);
}

int ZIP::add_file(const std::string &name, const void *content, size_t size, bool overwrite)
{
    auto source = zip_source_buffer(m_zip, content, size, 0);
    if (source == nullptr)
    {
        throw std::runtime_error(name + " error creating zip source");
    }
    // Interpret name as UTF-8.
    int flags = ZIP_FL_ENC_UTF_8;
    if (overwrite)
        flags |= ZIP_FL_OVERWRITE;

    // Result is file index in zip archive
    int idx = zip_file_add(m_zip, name.c_str(), source, flags);
    if (idx < 0)
    {
        std::string zip_err_str = zip_strerror(m_zip);
        zip_source_free(source);
        throw std::runtime_error(name + " error adding file: " + zip_err_str);
    }

    return idx;
}

int ZIP::add_dir(std::string name)
{
    if (name.back() != '/')
        name.push_back('/');
    auto idx = zip_dir_add(m_zip, name.c_str(), ZIP_FL_ENC_UTF_8);
    if (idx < 0)
    {
        throw std::runtime_error(name + " error creating directory");
    }

    return idx;
}

size_t ZIP::find_idx_by_name(const std::string &name) const
{
    auto idx = zip_name_locate(m_zip, name.c_str(), 0);
    if (idx < 0)
    {
        throw std::runtime_error(name + " not found in " + m_name);
    }
    return idx;
}


int ZIP::replace_file(const std::string &name, const void *content, size_t size)
{
    auto idx = find_idx_by_name(name);
    auto source = zip_source_buffer(m_zip, content, size, 0);
    if (source == nullptr)
    {
        throw std::runtime_error(name + " error creating zip source");
    }

    if (zip_file_replace(m_zip, idx, source, ZIP_FL_OVERWRITE) < 0)
    {
        throw std::runtime_error(name + " error replacing: " + zip_strerror(m_zip));
    }

    return idx;
}

void ZIP::delete_entry(size_t index)
{
    auto res = zip_delete(m_zip, index);
    if (res < 0)
    {
        throw std::runtime_error("error deleting entry");
    }
}

void ZIP::delete_entry(const std::string &name)
{
    delete_entry(find_idx_by_name(name));
}

size_t ZIP::get_num_entries() const
{

    return zip_get_num_entries(m_zip, 0);
}

std::string ZIP::get_name() const
{
    return m_name;
}

std::string ZIP::get_entry_name(size_t index) const
{
    auto name = zip_get_name(m_zip, index, 0);
    if (name == nullptr)
    {
        throw std::runtime_error("error obtaining file name");
    }
    return name;
}

zip_stat_t ZIP::get_entry_stat(size_t index) const
{
    zip_stat_t stat;
    if (zip_stat_index(m_zip, index, 0, &stat) != 0)
    {
        throw std::runtime_error("Cannot stat file");
    }
    return stat;
}

zip_stat_t ZIP::get_entry_stat(const std::string &name) const
{
    zip_stat_t stat;
    if (zip_stat(m_zip, name.c_str(), 0, &stat) != 0)
    {
        throw std::runtime_error("Cannot stat file " + name);
    }
    return stat;
}

size_t ZIP::get_file_size(size_t index) const
{
    auto stat = get_entry_stat(index);
    return stat.size;
}

size_t ZIP::get_file_size(const std::string &name) const
{
    auto stat = get_entry_stat(name);
    return stat.size;
}

std::unique_ptr<ZIP_Entry> ZIP::get_entry(size_t index) const
{
    auto stat = get_entry_stat(index);
    std::string name = stat.name;
    auto zf = zip_fopen_index(m_zip, index, 0);

    if (zf == nullptr)
    {
        throw std::runtime_error(name + " error creating zip entry");
    }
    return std::unique_ptr<ZIP_Entry>(new ZIP_Entry(name, zf));
}

std::unique_ptr<ZIP_Entry> ZIP::get_entry(const std::string &name) const
{
    return get_entry(find_idx_by_name(name));
}

std::list<std::unique_ptr<ZIP_Entry>> ZIP::get_entries() const
{
    std::list<std::unique_ptr<ZIP_Entry>> res;
    auto num_entries = get_num_entries();
    for (size_t i = 0; i < num_entries; ++i)
    {
        auto stat = get_entry_stat(i);
        if (stat.valid & ZIP_STAT_NAME && stat.name == nullptr) {
           // is deleted 
        }else {
            res.emplace_back(get_entry(i));
        }
    }
    return res;
}

bool ZIP::close() noexcept
{
    if (zip_close(m_zip) == 0)
        m_zip = nullptr;
    return m_zip == nullptr;
}

/**
 * OZIP proxy for simple W operations
 */

OZIP::OZIP(std::string name, int mode)
    : ZIP(std::move(name), mode)
{}

int OZIP::add_text_file(const std::string &name, const std::string &content, bool overwrite)
{
    return add_file(name, content.c_str(), content.size(), overwrite);
}
/**
 * IZIP proxy for read operations
 */

IZIP::IZIP(std::string name)
    : ZIP(std::move(name), ZIP_RDONLY)
{}
