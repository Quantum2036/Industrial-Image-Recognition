#include "TClassifier.h"
#include <io.h>
#include "Target.h"
#include <algorithm>

String TClassifier::strfeatureName[] = {
	"size",
	"peripheral",
	"major_axis",
	"minor_axis",
	"isHollow",
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

void TClassifier::SaveClassifier(const char* fileName)
{
	bool flag = LoadClassifier(fileName);
	if (flag == false) {
		printf("在%s创建新的分类器文件\n\n", fileName);
	}

	FILE* fp = OpenFile(fileName, "w+");

	fprintf_s(fp, "%zu\n\n", count);
	for (size_t i = 0; i < count; i++)
	{
		WriteStructure(fp, i);
	}

	fclose(fp);
}

void TClassifier::SaveClassifier(void)
{
	if (filePath.empty())
	{
		fprintf_s(stderr, "ERROR: 未找到分类器文件保存位置\n");
		return;
	}

	SaveClassifier(filePath.c_str());
}

bool TClassifier::LoadClassifier(const char* fileName)
{
	FILE* fp = nullptr;
	fopen_s(&fp, fileName, "r");
	if (!fp) {
		fprintf_s(stderr, "ERROR: 未找到分类器文件:%s\n", fileName);
		return false;
	}
	else {
		//保存分类器路径
		filePath = String(fileName);

		size_t length;	//分类器包含的特征数目
		fscanf_s(fp, "%zu\n\n", &length);
		for (size_t i = 0; i < length; i++)
		{
			ReadStructure(fp);
		}

		PrintClassifierMSG();
		fclose(fp);

		return true;
	}
}

void TClassifier::AddNewClass(const feature& TFea, const String& name)
{
	if (IsExistName(name)) {
		fprintf_s(stderr, "WARNING: 已存在命名\t[%s]\n", name.c_str());
		return;
	}
	else if (IsExistTFeature(TFea, 0.10)) {
		fprintf_s(stderr, "WARNING: 已存在记录的特征标识\t[%s]\n", classify(TFea).c_str());
		return;
	}

	data.emplace_back(TFea);
	Tname.emplace_back(name);
	count++;
}

String TClassifier::classify(const feature& TFea)
{
	fea_array feature_data = Target::Struct2Array(TFea);
	std::vector<fea_array> feature_list(count);
	for (size_t i = 0; i < count; i++) {
		feature_list.at(i) = Target::Struct2Array(data.at(i));
	}

	//得到待分类特征与记录特征的误差表
	for (auto it = feature_list.begin(); it != feature_list.end(); it++)
	{
		for (size_t i = 0; i < FEATURE_SIZE; i++) {
			(*it)[i] = fabs( (*it)[i] - feature_data[i] );
		}
	}

	//寻找综合误差最小的记录
	std::vector<size_t> score_sum(count);
	struct _key_value { size_t key; double value; };
	std::vector<struct _key_value> sorted(count);
	for (size_t order = 0; order < FEATURE_SIZE; order++)
	{
		for (size_t i = 0; i < count; i++)
		{
			sorted.at(i).key = i;
			sorted.at(i).value = feature_list.at(i)[order];
		}

		std::sort(
			sorted.begin(),
			sorted.end(),
			[](struct _key_value a, struct _key_value b) {return a.value < b.value; });

		size_t score = count;
		for (auto it = sorted.cbegin(); it != sorted.cend(); it++)
		{
			score_sum.at(it->key) += score;
			score--;
		}
	}

	size_t max_key = std::distance(
									score_sum.cbegin(),
									std::max_element(score_sum.cbegin(), score_sum.cend()));

	return Tname[max_key];
}

bool TClassifier::IsExistTFeature(const feature& TFea, double errlimit)
{
	for (auto it = data.cbegin(); it != data.cend(); it++) {
		if (IsFeatureEqual(TFea, *it, errlimit)) {
			return true;
		}
	}

	return false;
}

bool TClassifier::IsExistName(const String& name)
{
	for (auto it = Tname.cbegin(); it < Tname.cend(); it++) {
		if (!name.compare(*it)) {
			return true;
		}
	}

	return false;
}

void TClassifier::PrintTClassifier_d(void)
{
	fprintf_s(stdout, "分类器总数: %zu\n\n", count);
	for (size_t i = 0; i < count; i++) {
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

	TFea_in.isHollow == TFea_std.isHollow ? flag += 2 : flag -= 2;

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
	fscanf_s(fp, "\t%s = %lf;\n", cname, 256, &thisTFea.Rectangularity);
	fscanf_s(fp, "\t%s = %lf;\n", cname, 256, &thisTFea.consistency);
	fscanf_s(fp, "\t%s = %lf;\n", cname, 256, &thisTFea.eccentricity);
	fscanf_s(fp, "}\n\n");

	AddNewClass(thisTFea, tname);
}

void TClassifier::WriteStructure(FILE* fp, size_t num)
{	
	/// <note>
	/// 第二个参数不直接传结构的理由：
	/// 写入时要同时获取这个特征的结构和命名
	/// 不要想着优化了
	/// </note>

	String tname = Tname.at(num);
	feature fea = data.at(num);

	fprintf_s(fp, "\" %s \" = {\n", tname.c_str());
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[0].c_str(), fea.size);
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[1].c_str(), fea.peripheral);
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[2].c_str(), fea.major_axis);
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[3].c_str(), fea.minor_axis);
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[4].c_str(), fea.isHollow);
	fprintf_s(fp, "\t%s = %lf;\n", strfeatureName[5].c_str(), fea.Rectangularity);
	fprintf_s(fp, "\t%s = %lf;\n", strfeatureName[6].c_str(), fea.consistency);
	fprintf_s(fp, "\t%s = %lf;\n", strfeatureName[7].c_str(), fea.eccentricity);
	fprintf_s(fp, "}\n\n");
}

void TClassifier::PrintClassifierMSG(void)
{
	//输出读取的特征数目
	fprintf_s(stdout, "成功读入分类器，发现已记录特征[%zu]个:\n", count);

	//输出读取的特征名称
	for (size_t i = 0; i < count; i++) {
		fprintf_s(stdout, "\tNO.%zu\t%s\n", i+1, Tname[i].c_str());
	}
	puts("");
}
