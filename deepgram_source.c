#include <obs-module.h>

struct deepgram_source_data {
	obs_source_t *context;
	char *audio_source_name;
};

static const char *deepgram_source_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("Deepgram Source");
}

static void audio_capture(void *param, obs_source_t *source,
			      const struct audio_data *audio_data, bool muted)
{
    //blog(LOG_WARNING, "Running audio_capture.");

	struct deepgram_source_data *dgsd = param;
	UNUSED_PARAMETER(source);

    if (audio_data != NULL) {
    	//blog(LOG_WARNING, "%d", audio_data->frames);
    }
}

static void deepgram_source_update(void *data, obs_data_t *settings)
{
	blog(LOG_WARNING, "Running deepgram_source_update.");

    struct deepgram_source_data *dgsd = data;
	const char *audio_source_name = obs_data_get_string(settings, "audio_source_name");

    if (strcmp(dgsd->audio_source_name, audio_source_name) != 0) {
        dgsd->audio_source_name = bstrdup(audio_source_name);
        if (strcmp(audio_source_name, "none") != 0) {
            obs_source_t *audio_source = obs_get_source_by_name(audio_source_name);
            obs_source_add_audio_capture_callback(audio_source, audio_capture, dgsd);
        }
    }
}

static void *deepgram_source_create(obs_data_t *settings, obs_source_t *source)
{
	blog(LOG_WARNING, "Running deepgram_source_create.");

	struct deepgram_source_data *dgsd = bzalloc(sizeof(struct deepgram_source_data));
	dgsd->context = source;
    dgsd->audio_source_name = "none";
	deepgram_source_update(dgsd, settings);

    //audio_output_get_info(obs_get_audio()).format;
    //audio_output_get_sample_rate(obs_get_audio());
    //audio_output_get_channels(obs_get_audio());

    return(dgsd);
}

static void deepgram_source_destroy(void *data)
{
	blog(LOG_WARNING, "Running deepgram_source_destroy.");

    struct deepgram_source_data *dgsd = data;
	bfree(dgsd->audio_source_name);
	bfree(dgsd);
}

static void deepgram_source_render()
{
}

static uint32_t deepgram_source_width()
{
	return 100;
}

static uint32_t deepgram_source_height()
{
	return 100;
}

struct sources_and_parent {
	obs_property_t *sources;
	obs_source_t *parent;
};

static bool add_sources(void *data, obs_source_t *source)
{
	struct sources_and_parent *info = data;
	uint32_t caps = obs_source_get_output_flags(source);

	if (source == info->parent)
		return true;
	if ((caps & OBS_SOURCE_AUDIO) == 0)
		return true;

	const char *name = obs_source_get_name(source);
	obs_property_list_add_string(info->sources, name, name);
	blog(LOG_WARNING, name);

	return true;
}

static obs_properties_t *deepgram_source_properties(void *data)
{
	blog(LOG_WARNING, "Running deepgram_source_properties.");

/*
	obs_properties_t *properties = obs_properties_create();

	obs_source_t *parent = NULL;

    blog(LOG_WARNING, "A");

	obs_properties_add_color_alpha(properties, "color",
				       obs_module_text("ColorSource.Color"));

    obs_properties_add_text(properties, "audio_source_name", obs_module_text("Device"), OBS_TEXT_DEFAULT);
    blog(LOG_WARNING, "B");

	//obs_property_t *devices = obs_properties_add_list(
	//	properties, "somethingelse", obs_module_text("Device"),
	//	OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);

	obs_property_list_add_string(devices, "whatever", "anything");
//	obs_property_list_add_string(devices, obs_module_text("None"), "none");

	//struct sources_and_parent info = {devices, parent};
	//obs_enum_sources(add_sources, &info);


    blog(LOG_WARNING, "C");

	return properties;
*/

    struct deepgram_source_data *dgsd = data;
	obs_properties_t *properties = obs_properties_create();
	obs_source_t *parent = NULL;
	obs_property_t *property;

	obs_property_t *sources = obs_properties_add_list(
		properties, "audio_source_name", obs_module_text("Device"),
		OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(sources, obs_module_text("None"), "none");
	struct sources_and_parent info = {sources, parent};
	obs_enum_sources(add_sources, &info);

	UNUSED_PARAMETER(data);
	return properties;
}

static void deepgram_source_defaults(obs_data_t *settings)
{
	blog(LOG_WARNING, "Running deepgram_defaults.");
	obs_data_set_default_string(settings, "audio_source_name", "none");
}

struct obs_source_info deepgram_source = {
	.id = "deepgram_source",
	.type = OBS_SOURCE_TYPE_INPUT,
	.output_flags = OBS_SOURCE_VIDEO,
	.get_name = deepgram_source_name,
	.create = deepgram_source_create,
	.destroy = deepgram_source_destroy,
	.update = deepgram_source_update,
    .video_render = deepgram_source_render,
    .get_width = deepgram_source_width,
    .get_height = deepgram_source_height,
	.get_defaults = deepgram_source_defaults,
	.get_properties = deepgram_source_properties
};
