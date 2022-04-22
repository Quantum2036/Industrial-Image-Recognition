#include "Trainer.h"
#include <io.h>

Trainer::Trainer()
{

}

Trainer::Trainer(Mat& background, const char* folderPath, const char* fileType)
{
	std::vector<String> testImgs = VaildFileList( getAllFile(folderPath, fileType) );

	size_t length = testImgs.size();
	fprintf_s(stdout, "����[%llu]���ɶ�ȡͼƬ�ļ�.\n", length);

	for (size_t i = 0; i < length; i++)
	{
		fprintf_s(stdout, "����ɨ���[%llu]��ͼƬ\t", i+1);
		Mat tImg = imread(testImgs[i]);
		CVApp tho(tImg, background);
		PushFeature(*tho.getTargets());
	}
	Analysis();
	//SaveData_d();
}

String Trainer::StrAddChar(const String& str, size_t length)
{
	String name = str;
	name.append("_");
	if (length < 26) {
		String s;
		char c = 65 + (char)length;
		s = c;
		name += s;
	}
	else {
		String number = std::to_string((long double)length);
		name += number;
	}
	return name;
}

void Trainer::FileSave(String name, std::vector<feature>& feaIn)
{
	String filePath(DATAFOLDER);
	filePath.append(name);
	filePath.append(".txt");

	FILE* fp = nullptr;
	fopen_s(&fp, filePath.c_str(), "w");
	if (!fp) {
		fprintf_s(stderr, "ERROR: Open file fail. \n");
		exit(-1);
	}

	fprintf_s(fp, "���\t�ܳ�\t���\t�����\t�ǵ���\t���ζ�\tԲ�ζ�\tƫ����\n");

	auto it = feaIn.begin();
	auto it_end = feaIn.end();
	for (; it != it_end; it++) {
		fprintf_s(fp, "%u\t", it->size);
		fprintf_s(fp, "%d\t", it->Peripheral);
		fprintf_s(fp, "%d\t", it->MER.width);
		fprintf_s(fp, "%d\t", it->MER.height);
		fprintf_s(fp, "%d\t", it->corners);
		fprintf_s(fp, "%.3lf\t", it->Rectangularity);
		fprintf_s(fp, "%.3lf\t", it->consistency);
		fprintf_s(fp, "%.3lf\n", it->eccentricity);
	}

	fclose(fp);
}

void Trainer::PushFeature(std::vector<Target>& target)
{
	auto it = target.begin();
	auto it_end = target.end();

	for (; it != it_end; it++) {
		if (it->GetTargetState() == TargetState::TS_Normal)
		{
			feature_Data.emplace_back(it->TFea.TFea);
		}
		else if (it->GetTargetState() == TargetState::TS_OutImage) {
			fprintf_s(stdout, "ɨ�赽��߽����壬����ȥ.\n\n");
		}
	}
}

void Trainer::SaveData_d(void)
{
	FILE* fp = nullptr;
	fopen_s(&fp, "C:\\Users\\29462\\Desktop\\nut.txt", "w");
	if (!fp) {
		fprintf_s(stderr, "ERROR: Open file fail. \n");
		exit(-1);
	}

	fprintf_s(fp, "���\t�ܳ�\t���\t�����\t�ǵ���\t���ζ�\tԲ�ζ�\tƫ����\n");

	auto it = feature_Data.begin();
	auto it_end = feature_Data.end();
	for (; it != it_end; it++) {
		fprintf_s(fp, "%u\t", it->size);
		fprintf_s(fp, "%d\t", it->Peripheral);
		fprintf_s(fp, "%d\t", it->MER.width);
		fprintf_s(fp, "%d\t", it->MER.height);
		fprintf_s(fp, "%d\t", it->corners);
		fprintf_s(fp, "%.3lf\t", it->Rectangularity);
		fprintf_s(fp, "%.3lf\t", it->consistency);
		fprintf_s(fp, "%.3lf\n", it->eccentricity);
	}

	fclose(fp);
}

std::vector<String> Trainer::getAllFile(String folderPath, String type)
{
	std::vector<String> files;

	// �ļ����
	intptr_t hFile = 0;
	// �ļ���Ϣ
	struct _finddata_t fileinfo;

	String p;

	if ((hFile = _findfirst(p.assign(folderPath).append("\\*" + type).c_str(), &fileinfo)) != -1) {
		do {
			// �����ļ���ȫ·��
			files.push_back(p.assign(folderPath).append("\\").append(fileinfo.name));

		} while (_findnext(hFile, &fileinfo) == 0); //Ѱ����һ�����ɹ�����0������-1

		_findclose(hFile);
	}

	return files;
}

std::vector<String> Trainer::VaildFileList(std::vector<String> fileList)
{
	size_t length = fileList.size();
	std::vector<String> vaildFile;

	for (size_t i = 0; i < length; i++)
	{
		if (imread(fileList[i]).data != nullptr) {
			vaildFile.emplace_back(fileList[i]);
		}
	}
	fileList.clear();

	return vaildFile;
}

void Trainer::Analysis(void)
{
	if (feature_Data.empty()) {
		return;
	}

	using vFeature = std::vector<feature>;
	std::vector<vFeature> featrue_List;
	bool flag = false;

	auto it = feature_Data.cbegin();
	auto it_end = feature_Data.cend();
	for (; it != it_end; it++) {

		size_t length_list = featrue_List.size();
		for (size_t i = 0; i < length_list; i++)
		{
			flag = TClassifier::IsFeatureEqual(*it, featrue_List[i][0], 0.25);
			if (flag) {		//����ĳ������з������ƣ����������뵽��������ѭ��
				featrue_List[i].emplace_back(*it);
				goto TAG_NEXTLOOP;
			}

		}
		
		//��������������ʱ��ִ��
		featrue_List.emplace_back();
		featrue_List[featrue_List.size() - 1].emplace_back(*it);

		//	������һ�����ѭ��Ϊ��һ��*it����
	TAG_NEXTLOOP: ;
	}

	String name("nut");
	size_t len = featrue_List.size();
	for (size_t i = 0; i < len; i++)
	{
		FileSave(StrAddChar(name, i), featrue_List[i]);
	}
}
