#include "TClassifier.h"
#include <io.h>

String TClassifier::strfeatureName[] = {
	"size",
	"peripheral",
	"MER.width",
	"MER.height",
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
}

TClassifier::~TClassifier()
{
	data.clear();
	Tname.clear();
	count = 0;
}

TClassifier& TClassifier::operator=(const TClassifier& tc)
{
	count	= tc.count;
	data	= tc.data;
	Tname	= tc.Tname;

	return *this;
}

void TClassifier::SaveClassifier(void)
{
	LoadClassifier(DATAFOLDER"Target_Classifier.txt");
	FILE* fp = OpenFile(DATAFOLDER"Target_Classifier.txt", "w+");

	fprintf_s(fp, "%llu\n\n", count);
	for (size_t i = 0; i < count; i++)
	{
		WriteStructure(fp, i);
	}

	fclose(fp);
}

void TClassifier::LoadClassifier(const char* filePath)
{
	FILE* fp = nullptr;
	fopen_s(&fp, filePath, "r");
	if (!fp) {
		fprintf_s(stderr, "未找到分类器文件. \n");
	}
	else {
		size_t length = 0;
		fscanf_s(fp, "%llu\n\n", &length);
		for (size_t i = 0; i < length; i++)
		{
			ReadStructure(fp);
		}

		fclose(fp);
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

	err = fabs((double)TFea_in.Peripheral - (double)TFea_std.Peripheral) / (double)TFea_std.Peripheral;
	err < errlimit ? flag++ : flag -= 2;

	err = fabs((double)TFea_in.MER.width - (double)TFea_std.MER.width) / (double)TFea_std.MER.width;
	err < errlimit ? flag++ : flag -= 2;

	err = fabs((double)TFea_in.MER.height - (double)TFea_std.MER.height) / (double)TFea_std.MER.height;
	err < errlimit ? flag++ : flag -= 2;

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
		exit(-1);
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
	fscanf_s(fp, "\t%s = %d;\n", cname, 256, &thisTFea.Peripheral);
	fscanf_s(fp, "\t%s = %d;\n", cname, 256, &thisTFea.MER.width);
	fscanf_s(fp, "\t%s = %d;\n", cname, 256, &thisTFea.MER.height);
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
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[1].c_str(), fea.Peripheral);
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[2].c_str(), fea.MER.width);
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[3].c_str(), fea.MER.height);
	fprintf_s(fp, "\t%s = %d;\n", strfeatureName[4].c_str(), fea.corners);
	fprintf_s(fp, "\t%s = %lf;\n", strfeatureName[5].c_str(), fea.Rectangularity);
	fprintf_s(fp, "\t%s = %lf;\n", strfeatureName[6].c_str(), fea.consistency);
	fprintf_s(fp, "\t%s = %lf;\n", strfeatureName[7].c_str(), fea.eccentricity);
	fprintf_s(fp, "}\n\n");

}
