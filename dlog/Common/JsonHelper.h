#pragma once

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/encodings.h"

#include "vector"

namespace rapidjson {

//使用UTF-16的doc
typedef GenericDocument<UTF16<>> DocumentW;
typedef GenericValue<UTF16<>> ValueW;
typedef GenericStringStream<UTF16<>> StringStreamW;
typedef GenericStringBuffer<UTF16<>> StringBufferW;

} // namespace rapidjson

//注.如果定义了#pragma execution_character_set("utf-8"),那么文件内容是UTF-8

namespace dxlib {

///-------------------------------------------------------------------------------------------------
/// <summary> 转换到Json的公共方法..</summary>
///
/// <remarks> Dx, 2019/1/23. </remarks>
///-------------------------------------------------------------------------------------------------
class JsonHelper
{
  public:
    ///-------------------------------------------------------------------------------------------------
    /// <summary>
    /// 保存json到文件.这个没有进行转码的处理.如果不定义下面的内容,那么就是GBK编码.
    /// #pragma execution_character_set("utf-8")
    /// 如果定义了那么代码中的文本才会是utf-8的文件.因此不应该使用这个函数,程序中统一使用UTF-16进行处理.
    /// </summary>
    ///
    /// <remarks> Dx, 2019/1/23. </remarks>
    ///
    /// <param name="filePath"> [in] 文件路径. </param>
    /// <param name="doc">      The document. </param>
    ///-------------------------------------------------------------------------------------------------
    static void save(const std::string& filePath, const rapidjson::Document& doc)
    {
        FILE* fp;
#if defined(_WIN32) || defined(_WIN64)
        fopen_s(&fp, filePath.c_str(), "wb"); // 非 Windows 平台使用 "w"
#elif defined(__linux__)
        fp = fopen(filePath.c_str(), "w");
#endif
        char writeBuffer[256];
        rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
        rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
        doc.Accept(writer);
        fclose(fp);
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 输入的是utf16的doc,但是保存成utf8的格式. </summary>
    ///
    /// <remarks> Dx, 2019/3/11. </remarks>
    ///
    /// <param name="filePath"> Full pathname of the file. </param>
    /// <param name="doc">      The document. </param>
    /// <param name="putBOM">   (Optional) True to put bom. </param>
    ///-------------------------------------------------------------------------------------------------
    static void save(const std::string& filePath, const rapidjson::DocumentW& doc, bool putBOM = false, bool isPretty = true)
    {
        using namespace rapidjson;
        FILE* fp;

#if defined(_WIN32) || defined(_WIN64)
        fopen_s(&fp, filePath.c_str(), "wb"); // 非 Windows 平台使用 "w"
#elif defined(__linux__)
        fp = fopen(filePath.c_str(), "w");
#endif
        char writeBuffer[256];
        FileWriteStream ws(fp, writeBuffer, sizeof(writeBuffer));
        typedef EncodedOutputStream<UTF8<>, FileWriteStream> EncodedOutputStream_UTF8;
        EncodedOutputStream_UTF8 os(ws, putBOM); // with BOM
        if (isPretty) {
            PrettyWriter<EncodedOutputStream_UTF8, UTF16<>, UTF8<>> writer(os);
            doc.Accept(writer);
        }
        else {
            Writer<EncodedOutputStream_UTF8, UTF16<>, UTF8<>> writer(os);
            doc.Accept(writer);
        }

        //UTFType type = UTFType::kUTF8;
        //char writeBuffer[256];
        //FileWriteStream bos(fp, writeBuffer, sizeof(writeBuffer));
        //typedef AutoUTFOutputStream<unsigned, FileWriteStream> OutputStream;
        //OutputStream eos(bos, type, putBOM);
        //Writer<OutputStream, UTF8<>, AutoUTF<unsigned>> writer;
        //doc.Accept(writer);

        fclose(fp);
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 从文件(不考虑编码转化的)读取json. </summary>
    ///
    /// <remarks> Dx, 2019/3/11. </remarks>
    ///
    /// <param name="filePath"> Full pathname of the file. </param>
    /// <param name="doc">      [in,out] The document. </param>
    ///-------------------------------------------------------------------------------------------------
    static void readFile(const std::string& filePath, rapidjson::Document& doc)
    {
        using namespace rapidjson;
        FILE* fp;
#if defined(_WIN32) || defined(_WIN64)
        fopen_s(&fp, filePath.c_str(), "rb"); // 非 Windows 平台使用 "r"
#elif defined(__linux__)
        fp = fopen(filePath.c_str(), "r");
#endif
        //这里不能使用AutoUTF,因为这里的文件可能不是UTF的编码会导致失败.
        char readBuffer[256];
        rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        doc.ParseStream(is);
        fclose(fp);
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 从文件UTF读取json. </summary>
    ///
    /// <remarks> Dx, 2019/3/11. </remarks>
    ///
    /// <param name="filePath"> Full pathname of the file. </param>
    /// <param name="doc">      [in,out] The document. </param>
    ///-------------------------------------------------------------------------------------------------
    static void readFile(const std::string& filePath, rapidjson::DocumentW& doc)
    {
        using namespace rapidjson;
        FILE* fp;
#if defined(_WIN32) || defined(_WIN64)
        fopen_s(&fp, filePath.c_str(), "rb"); // 非 Windows 平台使用 "r"
#elif defined(__linux__)
        fp = fopen(filePath.c_str(), "r");
#endif
        char readBuffer[256];
        FileReadStream bis(fp, readBuffer, sizeof(readBuffer));
        AutoUTFInputStream<unsigned, FileReadStream> eis(bis);
        doc.ParseStream<0, AutoUTF<unsigned>>(eis);
        fclose(fp);
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 转换doc到string. </summary>
    ///
    /// <remarks> Surface, 2019/3/11. </remarks>
    ///
    /// <param name="doc"> The document. </param>
    ///
    /// <returns> Doc as a std::string. </returns>
    ///-------------------------------------------------------------------------------------------------
    static inline std::string toStr(const rapidjson::Document& doc, bool isPretty = false)
    {
        using namespace rapidjson;
        StringBuffer sb;
        if (isPretty) {
            PrettyWriter<StringBuffer> writer(sb);
            doc.Accept(writer);
        }
        else {
            Writer<StringBuffer> writer(sb);
            doc.Accept(writer); // Accept() traverses the DOM and generates Handler events.
        }
        return std::string(sb.GetString());
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 转换doc到string. </summary>
    ///
    /// <remarks> Dx, 2019/3/11. </remarks>
    ///
    /// <param name="doc"> The document. </param>
    ///
    /// <returns> Doc as a std::wstring. </returns>
    ///-------------------------------------------------------------------------------------------------
    static inline std::wstring toStr(const rapidjson::DocumentW& doc, bool isPretty = false)
    {
        using namespace rapidjson;
        StringBufferW sb;
        if (isPretty) {
            PrettyWriter<StringBufferW, UTF16<>, UTF16<>> writer(sb);
            doc.Accept(writer);
        }
        else {
            Writer<StringBufferW, UTF16<>, UTF16<>> writer(sb);
            doc.Accept(writer); // Accept() traverses the DOM and generates Handler events.
        }
        return std::wstring(sb.GetString());
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> UTF8转换成UTF16. </summary>
    ///
    /// <remarks> Dx, 2019/3/11. </remarks>
    ///
    /// <param name="str"> The string. </param>
    ///
    /// <returns> A std::wstring. </returns>
    ///-------------------------------------------------------------------------------------------------
    static inline std::wstring utf8To16(const std::string& str)
    {
        using namespace rapidjson;
        StringStream source(str.c_str());
        StringBufferW target;
        bool hasError = false;
        while (source.Peek() != '\0')
            if (!Transcoder<UTF8<>, UTF16<>>::Transcode(source, target)) {
                hasError = true;
                break;
            }
        if (!hasError) {
            return target.GetString();
        }
        return std::wstring();
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> UTF16转换成UTF8. </summary>
    ///
    /// <remarks> Dx, 2019/3/11. </remarks>
    ///
    /// <param name="str"> The string. </param>
    ///
    /// <returns> A std::string. </returns>
    ///-------------------------------------------------------------------------------------------------
    static inline std::string utf16To8(const std::wstring& str)
    {
        using namespace rapidjson;
        StringStreamW source(str.c_str());
        StringBuffer target;
        bool hasError = false;
        while (source.Peek() != L'\0')
            if (!Transcoder<UTF16<>, UTF8<>>::Transcode(source, target)) {
                hasError = true;
                break;
            }
        if (!hasError) {
            return target.GetString();
        }
        return std::string();
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> Creat empty object document. </summary>
    ///
    /// <remarks> Surface, 2019/3/11. </remarks>
    ///
    /// <returns> A rapidjson::Document. </returns>
    ///-------------------------------------------------------------------------------------------------
    static inline rapidjson::Document creatEmptyObjectDoc()
    {
        rapidjson::Document document;
        document.SetObject();
        return document;
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> Creat empty array document. </summary>
    ///
    /// <remarks> Surface, 2019/3/11. </remarks>
    ///
    /// <returns> A rapidjson::Document. </returns>
    ///-------------------------------------------------------------------------------------------------
    static inline rapidjson::Document creatEmptyArrayDoc()
    {
        rapidjson::Document document;
        document.SetArray();
        return document;
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> Creat empty object document. </summary>
    ///
    /// <remarks> Surface, 2019/3/11. </remarks>
    ///
    /// <returns> A rapidjson::Document. </returns>
    ///-------------------------------------------------------------------------------------------------
    static inline rapidjson::DocumentW creatEmptyObjectDocW()
    {
        rapidjson::DocumentW document;
        document.SetObject();
        return document;
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 从json读取并创建Doc. </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///
    /// <param name="text"> The text. </param>
    ///
    /// <returns> A rapidjson::DocumentW. </returns>
    ///-------------------------------------------------------------------------------------------------
    static inline rapidjson::DocumentW creatDocW(const std::wstring& text)
    {
        rapidjson::DocumentW document;
        //从string读取
        rapidjson::StringStreamW s(text.c_str());
        document.ParseStream(s);
        return document;
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 从json读取并创建Doc. </summary>
    ///
    /// <remarks> Dx, 2019/3/12. </remarks>
    ///
    /// <param name="text"> The text. </param>
    ///
    /// <returns> A rapidjson::DocumentW. </returns>
    ///-------------------------------------------------------------------------------------------------
    static inline rapidjson::DocumentW readJson(const std::wstring& text)
    {
        rapidjson::DocumentW document;
        //从string读取
        rapidjson::StringStreamW s(text.c_str());
        document.ParseStream(s);
        return document;
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary>
    /// Creat empty array document w.
    /// </summary>
    ///
    /// <remarks> Surface, 2019/3/11. </remarks>
    ///
    /// <returns> A rapidjson::DocumentW. </returns>
    ///-------------------------------------------------------------------------------------------------
    static inline rapidjson::DocumentW creatEmptyArrayDocW()
    {
        rapidjson::DocumentW document;
        document.SetArray();
        return document;
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> Creat array document w. </summary>
    ///
    /// <remarks> Surface, 2019/3/11. </remarks>
    ///
    /// <typeparam name="T"> Generic type parameter. </typeparam>
    /// <param name="obj"> The object. </param>
    ///
    /// <returns> A rapidjson::DocumentW. </returns>
    ///-------------------------------------------------------------------------------------------------
    static inline rapidjson::DocumentW creatArrayDocW(const std::vector<std::wstring>& obj)
    {
        rapidjson::DocumentW doc;
        doc.SetArray();
        auto& allocator = doc.GetAllocator();
        for (size_t i = 0; i < obj.size(); i++) {
            rapidjson::ValueW item;
            item.SetString(obj[i].c_str(), allocator);
            doc.PushBack(item, allocator);
        }
        return doc;
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> Creat array document w. </summary>
    ///
    /// <remarks> Surface, 2019/3/11. </remarks>
    ///
    /// <typeparam name="T"> Generic type parameter. </typeparam>
    /// <param name="obj"> The object. </param>
    ///
    /// <returns> A rapidjson::DocumentW. </returns>
    ///-------------------------------------------------------------------------------------------------
    template <typename T>
    static inline rapidjson::DocumentW creatArrayDocW(const std::vector<T>& obj)
    {
        rapidjson::DocumentW doc;
        doc.SetArray();
        auto& allocator = doc.GetAllocator();
        for (size_t i = 0; i < obj.size(); i++) {
            rapidjson::ValueW item;
            doc.PushBack(obj[i], allocator);
        }
        return doc;
    }
};
} // namespace dxlib