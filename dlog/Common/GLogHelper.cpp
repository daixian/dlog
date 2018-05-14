#include <stdlib.h>
#include "GLogHelper.h"
#include <io.h>
#include <windows.h>
#include <ShlObj.h> //SHGetSpecialFolderPath
#include <experimental\filesystem>

#ifdef _DEBUG
#pragma comment(lib, "glogd.lib")
#elif NDEBUG
#pragma comment(lib, "glog.lib")
#endif // _DEBUG

//���������־��Ŀ¼��
//#define LOGDIR "log"
//#define MKDIR "mkdir -p " LOGDIR
//#define MKDIR_WIN "mkdir " LOGDIR //windows�Ĳ���Ҫ-p����

#define CSIDL_APPDATA 0x001a        // <user name>\Application Data

using std::experimental::filesystem::path;

//����Ϣ������������ļ��� LOG(ERROR)  windowdƽ̨������
void GLogHelper::SignalHandle(const char* data, int size)
{
    std::string str = std::string(data, size);
    /*
    std::ofstream fs("glog_dump.log",std::ios::app);
    fs<<str;
    fs.close();
    */
    LOG(ERROR) << str;
    //Ҳ����ֱ�������﷢���ʼ������֪ͨ��������������Ǳ��ص���εģ�ÿ�λص�ֻ���һ�д�����Ϣ������������ļ�¼���ļ���Ҳ��Ҫ>��׷��ģʽ���ɣ����������﷢�ʼ�����Ų��Ǻ��ʺϣ��������ǿ��Ե���һ�� SHELL �� PYTHON �ű������˽ű����� sleep 3�����ң�Ȼ�󽫴�
    //����Ϣͨ���ʼ�����ŷ��ͳ�ȥ�������Ͳ���Ҫ��ؽű���ʱ��Ƶ��ִ�У��˷�Ч���ˡ�
}

bool GLogHelper::dirExists(const std::string& dirName_in)
{
    int ftyp = _access(dirName_in.c_str(), 0);

    if (0 == ftyp)
        return true;   // this is a directory!
    else
        return false;    // this is not a directory!
}

bool GLogHelper::dirExists(const std::wstring& dirName_in)
{
    int ftyp = _access(ws2s(dirName_in).c_str(), 0);

    if (0 == ftyp)
        return true;   // this is a directory!
    else
        return false;    // this is not a directory!
}

std::string GLogHelper::getAppDir()
{
    return getModuleDir();
}

std::string GLogHelper::isExistsAndCreat(std::wstring dirPath)
{
    std::string sDir = ws2s(dirPath);
    if (!dirExists(sDir))//����ļ���·��������
    {
        std::string cmd = std::string("mkdir \"") + sDir + std::string("\"");//���������Դ���һ���ļ���
        system(cmd.c_str()); //�����ļ���
    }
    return sDir;
}

std::string GLogHelper::isExistsAndCreat(std::string sDir)
{
    if (!dirExists(sDir))//����ļ���·��������
    {
        std::string cmd = std::string("mkdir \"") + sDir + std::string("\"");//���������Դ���һ���ļ���
        system(cmd.c_str()); //�����ļ���
    }
    return sDir;
}

//GLOG���ã�
GLogHelper::GLogHelper(char* program, char* logDir)
{
    programName = program;//��¼������

    google::InitGoogleLogging(program);

    //std::wstring path = getMyDirectory();//path = L"D:\\Work\\F3DSys\\F3DSystem"
    wchar_t *szPath = new wchar_t[512];

    //������������ã����ڹ���Աģʽ��·�����ǵ�ǰ�ļ���
    BOOL bRet = SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);//L"C:\\Users\\f3d\\AppData\\Roaming"

    logDirPath.clear();
    path dir = path(logDir);

    if (dir.is_absolute())
    {
        logDirPath = logDir;
    }
    else
    {
        logDirPath = getModuleDir();
        if (logDir[0] != '\\')
        {
            logDirPath.append("\\");
        }
        logDirPath.append(logDir);
    }
    if (!dirExists(logDirPath))//����ļ���·��������
    {
        std::string cmd = std::string("mkdir \"") + logDirPath + std::string("\"");
        system(cmd.c_str()); //�����ļ���
    }

    FLAGS_log_dir = logDirPath;// ������־�ļ�·��

    //google::SetLogFilenameExtension("91_");     //�����ļ�����չ����ƽ̨����������Ҫ���ֵ���Ϣ(��һ��ʵ�ʲ������ļ�����չ��)

    //FLAGS_log_dir = "log";// ������־�ļ�·�� "./log"
    //google::SetLogDestination(google::GLOG_INFO, "./log/info");// ����ĳ���������־�Ļ����ļ�·��

    //FLAGS_stderrthreshold = google::GLOG_WARNING; // ����־������ڵ��ڴ˼���ʱ���Զ�������־�������׼������
    FLAGS_stderrthreshold = google::GLOG_INFO; // ����־������ڵ��ڴ˼���ʱ���Զ�������־�������׼����(�ն˴���)��

    FLAGS_logbuflevel = google::GLOG_INFO;//��С�ڵ��ڻỺ��,����־������ڴ˼���ʱ�������������������
    FLAGS_colorlogtostderr = true;//���ò�ɫ����̨���
    FLAGS_logbufsecs = 1;  // ������ó�ʱ��Ϊ���
    FLAGS_max_log_size = 50;// ����־�ļ��ﵽ����ʱ�������ļ��ָ��MΪ��λ
    FLAGS_stop_logging_if_full_disk = false; // ����������ʱ,ֹͣ�����־�ļ�

    google::InstallFailureSignalHandler();      //��׽ core dumped
    google::InstallFailureWriter(SignalHandle);    //Ĭ�ϲ�׽ SIGSEGV �ź���Ϣ���������� stderr������ͨ������ķ����Զ������>��ʽ��

    std::wstring wsPath(szPath);

    LOG(INFO) << "APPDATA_DiR=" << ws2s(wsPath);
    LOG(INFO) << "log_dir=" << FLAGS_log_dir;
    delete[]szPath;
}

//GLOG�ڴ�����
GLogHelper::~GLogHelper()
{
    try {
        google::FlushLogFiles(google::GLOG_INFO);
        google::ShutdownGoogleLogging();
    }
    catch (std::string *caught)
    {
    }
}