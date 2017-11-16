#include "objLoader.h"
//实现objLoader.h
bool objLoader::loadobj(string filename)
{
	int Status = 0;
	ifstream file(filename);
	if (!file.is_open())
		return false;
	string objfile, head;
	while (getline(file,objfile))//读入obj文件
	{
		Status = 0;
		if (objfile[0] == 'v')
		{
			if (objfile[1] == 't')
			{
			}
			else if (objfile[1] == 'n')
			{
				istringstream in(objfile);
				in >> head >> a.x >> a.y >> a.z;
				vnArr.push_back(a);
			}
			else
			{
				istringstream in(objfile);
				in >> head >> a.x >> a.y >> a.z;
				vArr.push_back(a);
			}
		}
		else if (objfile[0] == 'f')
		{
			for (int k = objfile.size() - 1; k >= 0; k--) {
				if (objfile[k] == '/'&& objfile[k - 1] == '/')
				{
					Status++;
					objfile[k] = ' ';
					objfile.insert(k, "0");
				}
				else if (objfile[k] == '/')
				{
					Status++;
					objfile[k] = ' ';
				}
			}
			if (Status == 6)
			{
				istringstream in(objfile);
				in >> head >> a.x >> b.x >> c.x >> a.y >> b.y >> c.y >> a.z >> b.z >> c.z;
				f.push_back(face{ a,b,c });
			}
			else if (Status == 8)
			{
				istringstream in(objfile);
				in >> head >> a.x >> b.x >> c.x >> a.y >> b.y >> c.y >> a.z >> b.z >> c.z;
				f.push_back(face{ a,b,c });
				a.y = a.z; b.y = b.z; c.y = c.z;
				in >> a.z >> b.z >> c.z;
				f.push_back(face{ a,b,c });
			}
			else if (Status == 0)
			{
				istringstream in(objfile);
				in >> head >> a.x >> a.y >> a.z;
				f.push_back(face{ a,b,c });
			}
		}
		else
			continue;
	}
	return true;
}

