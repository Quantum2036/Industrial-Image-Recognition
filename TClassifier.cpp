#include "TClassifier.h"
#include <io.h>

String TClassifier::strfeatureName[] = {
	"size",
	"peripheral",
	"major_axis",
	"minor_axis",
	"isHollow",
	"corners",
	"rectangularity",
	"consistency",
	"eccentricity"
};

TClassifier::TClassifier()
{
	count = 0;
}

TClassifier::TClassifier(const TClassifier& tc)
{
	count	= tc.count;
	data	= tc.data;
	Tname	= tc.Tname;
	filePath = tc.filePath;
}

TClassifier::~TClassifier()
{
	data.clear();
	Tname.clear();
	count = 0;
	filePath.clear();
}

TClassifier& TClassifier::operator=(const TClassifier& tc)
{
	count	= tc.count;
	data	= tc.data;
	Tname	= tc.Tname;
	filePath = tc.filePath;
	return *this;
}

void TClassifier::ChangeName(String name)
{
	for (size_t i = 0; i < count; i++) {
		size_t pos = Tname[i].find("unnamed");
		if (pos == String::npos) {
			continue;
		}



	}
}

void TClassifier::SaveClassifier(const char* fileName)
{
	bool flag = LoadClassifier(fileName);
	if (flag == false) {
		printf("在%s创建新的分类器文件\n\n", fileName);
	}

	FILE* fp = OpenFile(fileName, "w+");

	fprintf_s(fp, "%llu\n\n", count);
	for (size_t i = 0; i < count; i++)
	{
		WriteStructure(fp, i);
	}

	fclose(fp);
}

void TClassifier::SaveClassifier(void)
{
	bool flag = LoadClassifier(DATAFOLDER"Target_Classifier.txt");
	FILE* fp = OpenFile(DATAFOLDER"Target_Classifier.txt", "w+");

	fprintf_s(fp, "%llu\n\n", count);
	for (size_t i = 0; i < count; i++)
	{
		WriteStructure(fp, i);
	}

	fclose(fp);
}

bool TClassifier::LoadClassifier(const char* fileName)
{
	FILE* fp = nullptr;
	fopen_s(&fp, fileName, "r");
	if (!fp) {
		fprintf_s(stderr, "未找到分类器文件. \n");
		return false;
	}
	else {
		//保存分类器路径
		filePath = String(fileName);

		size_t length = 0;	//分类器包含的特征数目
		fscanf_s(fp, "%llu\n\n", &length);
		for (size_t i = 0; i < length; i++)
		{
			ReadStructure(fp);
		}

		PrintClassifierMSG();
		fclose(fp);

		return true;
	}
}

void TClassifier::AddNewClass(feature TFea, String name)
{
	if (IsExistName(name)) {
		fprintf_s(stderr, "WARNING: 已存在命名\t[%s]\n", name.c_str());
		return;
	}
	else if (IsExistTFeature(TFea, 0.10)) {
		fprintf_s(stderr, "WARNING: 已存在记录的特征标识\t[%s]\n", getTC(TFea).c_str());
		return;
	}

	data.emplace_back(TFea);
	Tname.emplace_back(name);
	count++;
}

String TClassifier::getTC(const feature& TFea)
{
	for (size_t i = 0; i < count; i++) {
		if (IsFeatureEqual(TFea, data.at(i), 0.14)) {
			return Tname.at(i);
		}
	}
	return String("unknown");
}

String TClassifier::classify(const feature& TFea)
{
	std::vector<uint> arr(count, 0);
	
	double	big_number = 1E15;
	double	min_err = big_number;	//最小误差，进行比较前先设置为一极大数
	double	err = 0.0;				//记录的当前误差

	//将变量重置
	auto lfunc_reset = [&] {min_err = big_number;};
	auto minus = [](uint x, uint y) -> double {return (double)x - (double)y; };
	
	//面积
	for (size_t i = 0; i < count; i++)
	{
		err = fabs(minus(data[i].size,TFea.size));
		if (err < min_err) min_err = err;
	}
	for (size_t i = 0; i < count; i++) {
		if (fabs(minus(data[i].size, TFea.size)) == min_err) arr[i]++;
	}

	//周长
	lfunc_reset();
	for (size_t i = 0; i < count; i++)
	{
		err = fabs(minus(data[i].peripheral, TFea.peripheral));
		if (err < min_err) min_err = err;
	}
	for (size_t i = 0; i < count; i++) {
		if (fabs(minus(data[i].peripheral, TFea.peripheral)) == min_err) arr[i]++;
	}

	//长轴
	lfunc_reset();
	for (size_t i = 0; i < count; i++)
	{
		err = fabs(minus(data[i].major_axis, TFea.major_axis));
		if (err < min_err) min_err = err;
	}
	for (size_t i = 0; i < count; i++) {
		if (fabs(minus(data[i].major_axis, TFea.major_axis)) == min_err) arr[i]++;
	}

	//短轴
	lfunc_reset();
	for (size_t i = 0; i < count; i++)
	{
		err = fabs(minus(data[i].minor_axis, TFea.minor_axis));
		if (err < min_err) min_err = err;
	}
	for (size_t i = 0; i < count; i++) {
		if (fabs(minus(data[i].minor_axis, TFea.minor_axis)) == min_err) arr[i]++;
	}

	//Hollow
	lfunc_reset();
	for (size_t i = 0; i < count; i++)
	{
		if (TFea.isHollow == data[i].isHollow) {
			arr[i] += 2;
		}
	}

	//角点
	lfunc_reset();
	for (size_t i = 0; i < count; i++)
	{
		err = fabs(minus(data[i].corners, TFea.corners));
		if (err < min_err) min_err = err;
	}
	for (size_t i = 0; i < count; i++) {
		if (fabs(minus(data[i].corners, TFea.corners)) == min_err) arr[i]++;
	}

	//矩形度
	lfunc_reset();
	for (size_t i = 0; i < count; i++)
	{
		err = fabs(data[i].Rectangularity - TFea.Rectangularity);
		if (err < min_err) min_err = err;
	}
	for (size_t i = 0; i < count; i++) {
		if (fabs(data[i].Rectangularity - TFea.Rectangularity) == min_err) arr[i]++;
	}

	//圆形度
	lfunc_reset();
	for (size_t i = 0; i < count; i++)
	{
		err = fabs(data[i].consistency - TFea.consistency);
		if (err < min_err) min_err = err;
	}
	for (size_t i = 0; i < count; i++) {
		if (fabs(data[i].consistency - TFea.consistency) == min_err) arr[i]++;
	}

	//偏心率
	lfunc_reset();
	for (size_t i = 0; i < count; i++)
	{
		err = fabs(data[i].eccentricity - TFea.eccentricity);
		if (err < min_err) min_err = err;
	}
	for (size_t i = 0; i < count; i++) {
		if (fabs(data[i].eccentricity - TFea.eccentricity) == min_err) arr[i]++;
	}

	//统计最高得分
	lfunc_reset();
	size_t num = 0;
	size_t max_score = 0;
	for (size_t i = 0; i < count; i++)
	{
		if (arr[i] > max_score) {
			max_score = arr[i];
			num = i;
		}
	}

	return Tname[num];
}

bool TClassifier::IsExistTFeature(const feature& TFea, double errlimit)
{
	for (int i = 0; i < count; i++) {
		if (IsFeatureEqual(TFea, data.at(i), errlimit)) {
			return true;
		}
	}

	return false;
}

bool TClassifier::IsExistName(const String& name)
{
	for (int i = 0; i < count; i++)	{
		if (!name.compare(Tname.at(i))) {
			return true;
		}
	}

	return false;
}

void TClassifier::PrintTClassifier_d(void)
{
	fprintf_s(stdout, "%llu\n\n", count);
	for (size_t i = 0; i < count; i++)
	{
		WriteStructure(stdout, i);
	}
}

bool TClassifier::IsFeatureEqual(const feature& TFea_in, const feature& TFea_std, double errlimit)
{
	double	err = 0.0;
	int		flag = 0;

	err = fabs((double)TFea_in.size - (double)TFea_std.size) / (double)TFea_std.size;
	err < errlimit ? flag++ : flag -= 2;

	err = fabs((double)TFea_in.peripheral - (double)TFea_std.peripheral) / (double)TFea_std.peripheral;
	err < errlimit ? flag++ : flag -= 2;

	err = fabs((double)TFea_in.major_axis - (double)TFea_std.major_axis) / (double)TFea_std.major_axis;
	err < errlimit ? flag++ : flag -= 2;

	err = fabs((double)TFea_in.minor_axis - (double)TFea_std.minor_axis) / (double)TFea_std.minor_axis;
	err < errlimit ? flag++ : flag -= 2;

	TFea_in.isHollow == TFea_std.isHollow ? flag++ : flag -= 4;

	err = fabs(TFea_in.corners - TFea_std.corners);
	err < 2.0 ? flag++ : flag -= 2;

	err = fabs(TFea_in.Rectangularity - TFea_std.Rectangularity) / TFea_std.Rectangularity;
	err < errlimit ? flag++ : flag -= 2;

	err = fabs(TFea_in.consistency - TFea_std.consistency) / TFea_std.consistency;
	err < errlimit ? flag++ : flag -= 2;

	err = fabs(TFea_in.eccentricity - TFea_std.eccentricity) / TFea_std.eccentricity;
	err < errlimit ? flag++ : flag -= 2;

	return flag > 0 ? true : false;
}

FILE* TClassifier::OpenFile(const char* ClassifierPath, const char* mode)
{
	FILE* fp = nullptr;
	fopen_s(&fp, ClassifierPath, mode);
	if (!fp) {
		fprintf_s(stderr, "ERROR: Open file fail. \n");
		exit(EXIT_FAILURE);
	}

	return fp;
}

void TClassifier::ReadStructure(FILE* fp)
{
	feature thisTFea;
	char tname[256] = {0};
	char cname[256] = {0};

	fscanf_s(fp, "\" %s \" = {\n", tname, 256);
	fscanf_s(fp, "\t%s = %d;\n", cname, 256, &thisTFea.size);
	fscanf_s(fp, "\t%s = %d;\n", cname, 256, &thisTFea.peripheral);
	fscanf_s(fp, "\t%s = %d;\n", cname, 256, &thisTFea.major_axis);
	fscanf_s(fp, "\t%s = %d;\n", cname, 256, &thisTFea.minor_axis);
	fscanf_s(fp, "\t%s = %d;\n", cname, 256, &thisTFea.isHollow);
	fscanf_s(fp, "\t%s = %d;\n", cname, 256, &thisTFea.corners);
	fscanf_s(fp, "\t%s = %lf;\n", cname, 256, &thisTFea.Rectangularity);
	fscanf_s(fp, "\t%s = %lf;\n", cname, 256, &thisTFea.consistency);
	fscanf_s(fp, "\t%s = %lf;\n", cname, 256, &thisTFea.eccentricity);
	fscanf_s(fp, "}\n\n");

	AddNewClass(thisTFea, tname);
}

void TClassifier::WriteStructure(FILE* fp, size_t count)
{	
	String tname = Tname.at(count);
	feature fea = data.at(count);

	fprintf_s(fp, "\" %s \" = {\n", tname.c_str());
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[0].c_str(), fea.size);
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[1].c_str(), fea.peripheral);
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[2].c_str(), fea.major_axis);
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[3].c_str(), fea.minor_axis);
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[4].c_str(), fea.isHollow);
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[5].c_str(), fea.corners);
	fprintf_s(fp, "\t%s = %lf;\n", strfeatureName[6].c_str(), fea.Rectangularity);
	fprintf_s(fp, "\t%s = %lf;\n", strfeatureName[7].c_str(), fea.consistency);
	fprintf_s(fp, "\t%s = %lf;\n", strfeatureName[8].c_str(), fea.eccentricity);
	fprintf_s(fp, "}\n\n");

}

void TClassifier::PrintClassifierMSG(void)
{
	//输出读取的特征数目
	printf("成功读入分类器，发现已记录特征[%llu]个:\n", count);

	//输出读取的特征名称
	for (size_t i = 0; i < count; i++) {
		printf("\tNO.%llu\t%s\n", i+1, Tname[i].c_str());
	}

	puts("");
}
