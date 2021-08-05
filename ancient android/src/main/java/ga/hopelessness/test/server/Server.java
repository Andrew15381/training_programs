package ga.hopelessness.test.server;

import android.graphics.drawable.*;
import com.google.gson.*;
import com.google.gson.reflect.*;
import ga.hopelessness.test.model.*;
import java.io.*;
import java.net.*;
import java.util.*;

/*Класс для работы с сервером*/
public class Server
{
	/*Путь к файлу*/
	private static final String basePath="http://www.hopelessness.ga/server.php?";

	/*Оуществляет GET-запрос к серверу.
	 parameters - параметры запроса*/
	private static String getResponse(Map<String, String> parameters) throws Exception
	{
		/*Добавляются параметры к строке запроса*/
		StringBuffer withParameters=new StringBuffer(basePath);
		for (Map.Entry<String, String> parameter:parameters.entrySet())
		{
			withParameters.append(parameter.getKey());
			withParameters.append("=");
			withParameters.append(parameter.getValue());
			withParameters.append("&&");
		}

		/*Осуществляется запрос к серверу и считывается результат*/
		HttpURLConnection connection = (HttpURLConnection)new URL(withParameters.toString()).openConnection();
		connection.setRequestMethod("GET");

		BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));
		StringBuffer response = new StringBuffer();
		String line;
		while ((line = reader.readLine()) != null)
		{
			response.append(line);
		}
		return response.toString();
	}

	/*Загружает тесты с сервера.*/
	public static ArrayList<TestModel> loadTests() throws Exception
	{
		Map<String, String> parameters = new HashMap<String, String>();
		parameters.put("action", "tests");

		/*Разбирается полученный JSON*/
		return new GsonBuilder().create().fromJson(getResponse(parameters), new TypeToken<ArrayList<TestModel>>(){}.getType());
	}

	/*Загружает задание с сервера.
	 themeId - тема задания*/
	public static TaskModel loadTask(String themeId) throws Exception
	{
		Map<String, String> parameters = new HashMap<String, String>();
		parameters.put("action", "task");
		parameters.put("theme", themeId);

		/*Разбирается полученный JSON*/
		return new GsonBuilder().create().fromJson(getResponse(parameters), TaskModel.class);
	}

	/*Загружает оценку за тест с сервера.
	 testId - тест
	 points - количество баллов*/
	public static String loadMark(String testId, int points) throws Exception
	{
		Map<String, String> parameters = new HashMap<String, String>();
		parameters.put("action", "mark");
		parameters.put("test", testId);
		parameters.put("points", Integer.toString(points));

		return getResponse(parameters);
	}
	
	/*Загружает тесты с сервера.
	  name - название теста для поиска*/
	public static ArrayList<TestModel> searchTest(String name) throws Exception
	{
		Map<String, String> parameters = new HashMap<String, String>();
		parameters.put("action", "search");
		parameters.put("name", name);

		/*Разбирается полученный JSON*/
		return new GsonBuilder().create().fromJson(getResponse(parameters), new TypeToken<ArrayList<TestModel>>(){}.getType());
	}

	/*Загружает картинку.
	 source - путь к картинке*/
	public static Drawable loadImage(String source) throws Exception
	{
		return BitmapDrawable.createFromStream(new URL(source).openStream(), "src");
	}
}
