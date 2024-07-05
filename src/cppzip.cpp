#include <stdexcept>

#include "cppzip.h"

ZIP_File::ZIP_File(zip_file_t *zf)
    : m_zf(zf)
{
    if (m_zf == nullptr)
    {
        throw std::runtime_error("Error creating zip file");
    }
}

ZIP_File::~ZIP_File()
{
    if (m_zf)
        zip_fclose(m_zf);
}

std::vector<char> ZIP_File::read(size_t size)
{
    std::vector<char> buffer(size);
    auto bytes_read = zip_fread(m_zf, buffer.data(), size);
    if (bytes_read < 0)
    {
        throw std::runtime_error("Error reading from zip file");
    }
    return buffer;
}

ZIP::ZIP(const std::string &name, int mode)
{
    int err;
    m_zip = zip_open(name.c_str(), mode, &err);
    if (m_zip == nullptr)
    {
        zip_error_t zipError;
        zip_error_init_with_code(&zipError, err);
        std::string zip_err_str = zip_error_strerror(&zipError);
        zip_error_fini(&zipError);
        throw std::runtime_error(name + " zip error " + zip_err_str);
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

size_t ZIP::get_num_entries() const
{
    auto num_files = zip_get_num_entries(m_zip, 0);
    if (num_files < 0)
    {
        throw std::runtime_error("error obtaining number of entries in zip");
    }

    return static_cast<size_t>(num_files);
}

std::string ZIP::get_file_name(size_t index)
{
    auto name = zip_get_name(m_zip, index, 0);
    if (name == nullptr)
    {
        throw std::runtime_error("error obtaining file name");
    }
    return name;
}

zip_stat_t ZIP::get_file_stat(size_t index)
{
    zip_stat_t stat;
    if (zip_stat_index(m_zip, index, 0, &stat) != 0)
    {
        throw std::runtime_error("Cannot stat file inside zip");
    }
    return stat;
}

size_t ZIP::get_file_size(size_t index)
{
    auto stat = get_file_stat(index);
    return stat.size;
}

std::unique_ptr<ZIP_File> ZIP::get_file(size_t index)
{
    return std::unique_ptr<ZIP_File>(new ZIP_File(zip_fopen_index(m_zip, index, 0)));
}

bool ZIP::close() {
    if (zip_close(m_zip) == 0) 
        m_zip = nullptr;
    return m_zip == nullptr;
}
